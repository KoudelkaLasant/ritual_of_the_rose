#pragma once
#include "Utils.h"


template<class Interface>
inline void
SafeRelease(
	Interface** ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

class Graphics {
public:
    Graphics() {}
    ~Graphics() {
        teardownAllImages();
        teardownAllTextFormats();
        SafeRelease(&D2DFactory);
        //SafeRelease(&IWICFactory);
        SafeRelease(&DWriteFactory);
        SafeRelease(&hwndRenderTarget);
    }
    class Drawable {
    public:
        D2D1_RECT_F getRect(pair<int, int> position, D2D1_SIZE_F size) {
            if (anchorStyle == "TOPLEFT") {
                return D2D1::RectF(position.first, position.second, position.first + size.width, position.second + size.height);
            }
            else {
                return D2D1::RectF(
                    position.first - (size.width / 2),
                    position.second - (size.height / 2),
                    position.first + (size.width / 2),
                    position.second + (size.height / 2));
            }
        }
        pair<int, int> getAbsolutePosition(D2D1_SIZE_F renderTargetSize) {
            return { renderTargetSize.width * positionAsPercentage.first / 100, renderTargetSize.height * positionAsPercentage.second / 100 };
        }
        pair<int, int> positionAsPercentage = { 0,0 };
        string anchorStyle;
        string unique_ID;
        bool operator==(const Drawable& rhs) const {
            return unique_ID == rhs.unique_ID;
        }
    };
	class Image : public Drawable {
	public:
        Image() {}
        Image(List<int> _sources, pair<int, int> position, string _anchorStyle, float _opacity, string _uniqueID) {
            sources = _sources;
            positionAsPercentage = position;
            anchorStyle = _anchorStyle;
            opacity = _opacity;
            unique_ID = _uniqueID;
        }
		~Image() {
                while (not textures.empty()) {
                    ID2D1Bitmap * toDelete = textures.front();
                    SafeRelease(& toDelete);
                    textures.front() = NULL;
                    textures.pop_front();
                }
		}
		bool isLoaded() {
			return getWhichTexture() != NULL;
		}
        HRESULT loadTexture(Graphics & graphics) {
            ID2D1Bitmap* texture = getWhichTexture();
            HRESULT hr = S_OK;
            IWICBitmapDecoder* pDecoder = NULL;
            IWICBitmapFrameDecode* pSource = NULL;
            IWICStream* pStream = NULL;
            IWICFormatConverter* pConverter = NULL;
            IWICBitmapScaler* pScaler = NULL;

            HRSRC imageResHandle = NULL;
            HGLOBAL imageResDataHandle = NULL;
            void* pImageFile = NULL;
            DWORD imageFileSize = 0;

            // Locate the resource.
            imageResHandle = FindResource(HINST_THISCOMPONENT, MAKEINTRESOURCE(sources.at(frame)), L"PNG");

            hr = imageResHandle ? S_OK : E_FAIL;
            if (SUCCEEDED(hr))
            {
                // Load the resource.
                imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);

                hr = imageResDataHandle ? S_OK : E_FAIL;
            }
            if (SUCCEEDED(hr))
            {
                // Lock it to get a system memory pointer.
                pImageFile = LockResource(imageResDataHandle);

                hr = pImageFile ? S_OK : E_FAIL;
            }
            if (SUCCEEDED(hr))
            {
                // Calculate the size.
                imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);

                hr = imageFileSize ? S_OK : E_FAIL;
            }
            if (SUCCEEDED(hr))
            {
                // Create a WIC stream to map onto the memory.
                hr = graphics.IWICFactory->CreateStream(&pStream);
            }
            if (SUCCEEDED(hr))
            {
                // Initialize the stream with the memory pointer and size.
                hr = pStream->InitializeFromMemory(
                    reinterpret_cast<BYTE*>(pImageFile),
                    imageFileSize
                );
            }
            if (SUCCEEDED(hr))
            {
                // Create a decoder for the stream.
                hr = graphics.IWICFactory->CreateDecoderFromStream(
                    pStream,
                    NULL,
                    WICDecodeMetadataCacheOnLoad,
                    &pDecoder
                );
            }
            if (SUCCEEDED(hr))
            {
                // Create the initial frame.
                hr = pDecoder->GetFrame(0, &pSource);
            }
            if (SUCCEEDED(hr))
            {
                // Convert the image format to 32bppPBGRA
                // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
                hr = graphics.IWICFactory->CreateFormatConverter(&pConverter);
            }
            if (SUCCEEDED(hr))
            {
                    UINT originalWidth, originalHeight;
                    hr = pSource->GetSize(&originalWidth, &originalHeight);
                    if (SUCCEEDED(hr))
                    {
                        hr = graphics.IWICFactory->CreateBitmapScaler(&pScaler);
                        if (SUCCEEDED(hr))
                        {
                            hr = pScaler->Initialize(
                                pSource,
                                originalWidth,
                                originalHeight,
                                WICBitmapInterpolationModeCubic
                            );
                            if (SUCCEEDED(hr))
                            {
                                hr = pConverter->Initialize(
                                    pScaler,
                                    GUID_WICPixelFormat32bppPBGRA,
                                    WICBitmapDitherTypeNone,
                                    NULL,
                                    0.f,
                                    WICBitmapPaletteTypeMedianCut
                                );
                            }
                        }
                }
                else
                {
                    hr = pConverter->Initialize(
                        pSource,
                        GUID_WICPixelFormat32bppPBGRA,
                        WICBitmapDitherTypeNone,
                        NULL,
                        0.f,
                        WICBitmapPaletteTypeMedianCut
                    );
                }
            }
            if (SUCCEEDED(hr))
            {
                //create a Direct2D bitmap from the WIC bitmap.
                hr = graphics.hwndRenderTarget->CreateBitmapFromWicBitmap(
                    pConverter,
                    NULL,
                    &texture
                );
            }
            textures.at(frame) = texture;

            SafeRelease(&pDecoder);
            SafeRelease(&pSource);
            SafeRelease(&pStream);
            SafeRelease(&pConverter);
            SafeRelease(&pScaler);

            return hr;
        }
        ID2D1Bitmap* getWhichTexture() {
            if (textures.empty()) {
                textures.push_back(NULL);
            }
            if (frame >= textures.size()) {
                frame = 0;
            }
            return textures.at(frame);
        }
        void draw(Graphics & graphics) {
            animate();
            ID2D1Bitmap* texture = getWhichTexture();
            if (texture == NULL) {
                loadTexture(*&graphics);
            }
            texture = textures.at(frame);
            D2D1_SIZE_F size = texture->GetSize();
            D2D1_SIZE_F renderTargetSize = graphics.hwndRenderTarget->GetSize();
            pair<int, int> position = getAbsolutePosition(renderTargetSize);
            D2D1_RECT_F rect = getRect(position, size);
            graphics.hwndRenderTarget->DrawBitmap(
                texture,
                rect, 
                opacity);
        }
        void animate() {
            if (!animated) { return; }
            if (animationStyles.contains("SINGLE") and frame<textures.size()-1) {
                frame++;
            }
            if (animationStyles.contains("LOOP")) { frame++; }
            if (animationStyles.contains("FADEOUT")) {
                if (CLOCK.hasEnoughTimePassed(unique_ID + "_FADE", 100)) {
                    opacity = TChange(opacity, -0.07f, 0.0f, 1.0f);
                }
            }
        };

        List<int> sources;
        int frame = 0;
        bool animated = false;
        List<string> animationStyles;
        float opacity = 1.0;
		List<ID2D1Bitmap  *> textures;
	};
    class Text : public Drawable {
    public:
        Text() {}
        Text(string message) {
            message = message;
        }
        Text(wstring _message, string _format, pair<int, int> _positionAsPercentage, string _anchorStyle, pair<int, int> _size, vector<float> _colour, string _unique_ID) {
            message = _message;
            format = _format;
            positionAsPercentage = _positionAsPercentage;
            anchorStyle = _anchorStyle;
            size = _size;
            colour = _colour;
            unique_ID = _unique_ID;
        }
        void draw(Graphics& graphics) {
            D2D1_SIZE_F renderTargetSize = graphics.hwndRenderTarget->GetSize();
            pair<int, int> position = getAbsolutePosition(renderTargetSize);
            D2D_SIZE_F size_as_d2d = D2D_SIZE_F();
            size_as_d2d.width = size.first;
            size_as_d2d.height = size.second;
            D2D1_RECT_F rect = getRect(position, size_as_d2d);
            ID2D1SolidColorBrush* theBrush = NULL;
            HRESULT hr = S_OK;
            hr = graphics.hwndRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF(colour[0], colour[1], colour[2], colour[3])),
                &theBrush);
            graphics.hwndRenderTarget->DrawText(
                message.c_str(),
                message.size(),
                graphics.WriteTextFormats[format],
                rect,
                theBrush);
            SafeRelease(&theBrush);
        }
        void startTypewriter() {
            fullMessage = message;
            message = L"";
        }

        wstring fullMessage;
        wstring message;
        string format;
        pair<int, int> size;
        vector<float> colour = {0.0,0.0,0.0,1.0};
    };
    void setup(HWND * hwnd) {
        hwnd = hwnd;
        CreateDeviceIndependentResources();
    }
    HRESULT CreateDeviceIndependentResources() {
        HRESULT hr;
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFactory);
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IWICImagingFactory,
            reinterpret_cast<void**>(&IWICFactory)
        );
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(DWriteFactory),
            reinterpret_cast<IUnknown**>(&DWriteFactory)
        );
        IDWriteTextFormat* textFormat = NULL;
        hr = DWriteFactory->CreateTextFormat(L"Times New Roman",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20,
            L"",
            &textFormat);
        if (SUCCEEDED(hr)) {
            WriteTextFormats.add({ "DEFAULT", textFormat });
        }
        filesystem::path path = filesystem::current_path() / "Cursor.cur";
        HCURSOR default_cursor = LoadCursorFromFileA(path.string().c_str());
        Cursors["DEFAULT"] = default_cursor;
        Colours["BLACK"] = { 0.0,0.0,0.0,1.0 };
        Colours["WHITE"] = { 1.0,1.0,1.0,1.0 };

        return hr;
    }
    HRESULT CreateDeviceResources() {
        HRESULT hr = S_OK;
        if (!hwndRenderTarget) {
            RECT rc;
            GetClientRect(*hwnd, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(static_cast<UINT>(rc.right - rc.left),static_cast<UINT>(rc.bottom - rc.top));
            
            hr = D2DFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(*hwnd, size),
                &hwndRenderTarget); 
        }
        return hr;
    }
    HRESULT OnRender() {
        HRESULT hr;
        hr = CreateDeviceResources();
        if (SUCCEEDED(hr) && !(hwndRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
        {
            hwndRenderTarget->BeginDraw();

            hwndRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

            hwndRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

            int latest_layer = -999;

            // draw layer x's texture after drawing layer x's images
            for (auto const& [key, val] : ImageMap.internalMap) {
                if (latest_layer < key) { latest_layer = key; }
                for (auto const& value : ImageMap[key].internalList) {
                    value->draw(*this);
                }
                for (auto & [subkey, subval] : TextMap[key].internalMap) {
                    subval.draw(*this);
                }
            }
            // draw any textures that are on higher layers than any existing images
            for (auto const& [key, val] : TextMap.internalMap) {
                if (key <= latest_layer) { continue; }
                for (auto& [subkey, subval] : TextMap[key].internalMap) {
                    subval.draw(*this);
                }
            }

            // add code here to draw every image 
        }
        hr = hwndRenderTarget->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET){
            hr = S_OK;
            DiscardDeviceResources();
        }
        return hr;
    }

    void DiscardDeviceResources()
    {
        SafeRelease(&hwndRenderTarget);
    }
    void addImage(Image * image, int layer) {
        if (not ImageMap.hasKey(layer)) {
            ImageMap[layer] = List<Image *>();
        }
        ImageMap[layer].push_back(image);
    }
    void addText(Text text, int layer) {
        if (not TextMap.hasKey(layer)) {
            TextMap[layer] = Map<string, Text>();
        }
        TextMap[layer][text.unique_ID] = text;
    }
    void teardownAllImages() {
        for (auto const & [key, val] : ImageMap.internalMap) {
            while (not ImageMap[key].empty()) {
                Image* toDelete = ImageMap[key].front();
                delete ImageMap[key].front();
                ImageMap[key].front() = NULL;
                ImageMap[key].pop_front();
            }
        }
        ImageMap.clear();
        
    }
    void teardownAllTextFormats() {
        for (auto const& [key, val] : WriteTextFormats.internalMap) {
            SafeRelease(&WriteTextFormats[key]);
        }
    }
    Image* accessImageViaUniqueID(string uniqueID) {
        for (auto const& [key, val] : ImageMap.internalMap) {
            for (int x = 0; x < ImageMap[key].internalList.size(); x++) {
                if (ImageMap[key].at(x)->unique_ID == uniqueID) {
                    return ImageMap[key].at(x);
                }
            }
        }
        ErrorHelper::warning({ uniqueID + " could not be found!" }, true);
    }
    void tearDownSpecifiedImage(string uniqueID) {
        int index = -1;
        int the_key = 0;
        Image* theImage = NULL;
        for (auto const& [key, val] : ImageMap.internalMap) {
            for (int x = 0; x < ImageMap[key].internalList.size(); x++) {
                if (ImageMap[key].at(x)->unique_ID == uniqueID) {
                    theImage = ImageMap[key].at(x);
                    index = x;
                    the_key = key;
                    break;
                }
            }
            
        }
        if (index != -1) {
            ImageMap[the_key].remove_at(index);
            delete theImage;
            theImage = NULL;
        }
    }
    void tearDownSpecifiedText(string uniqueID) {
        for (auto const& [key, val] : TextMap.internalMap) {
            if (TextMap[key].getKeys().contains(uniqueID)) {
                TextMap[key].internalMap.erase(uniqueID);
            }
        }
    }
    void tearDownAllCursors() {
        for (auto const& [key, val]: Cursors.internalMap) {
            DestroyCursor(val);
        }
        Cursors.clear();
    }

    bool does_this_text_already_exist(string uniqueID) {
        for (auto const& [key, val] : TextMap.internalMap) {
            if (TextMap[key].getKeys().contains(uniqueID)) {
                return true;
            }
        }
        return false;
    }

    HWND * hwnd;
	ID2D1Factory * D2DFactory;
    IWICImagingFactory * IWICFactory;
    Map<string, IDWriteTextFormat *> WriteTextFormats;

    IDWriteFactory* DWriteFactory;
    ID2D1HwndRenderTarget* hwndRenderTarget;
    Map<int, List<Image *>> ImageMap;
    Map<int, Map<string, Text>> TextMap;
    Map<string, HCURSOR> Cursors;
    Map<string, vector<float>> Colours;
};
Graphics graphics = Graphics();