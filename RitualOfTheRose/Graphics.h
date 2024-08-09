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
    Graphics() {
        colourTagLookupTable[wchar_t(10112)] = "BLUE";
        Colours["BLACK"] = { 0.0,0.0,0.0,1.0 };
        Colours["WHITE"] = { 1.0,1.0,1.0,1.0 };
        Colours["BLUE"] = { 0.0,0.0,1.0,1.0 };
    }
    ~Graphics() {
        teardownAllImages();
        teardownAllTextFormats();
        tearDownAllLoadedFonts();
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
        Text(wstring _message, string _format, pair<int, int> _positionAsPercentage, string _anchorStyle, pair<int, int> _size, vector<float> _colour, vector<float> _shadowColour, string _unique_ID) {
            message = _message;
            format = _format;
            positionAsPercentage = _positionAsPercentage;
            anchorStyle = _anchorStyle;
            size = _size;
            colour = _colour;
            unique_ID = _unique_ID;
            shadowColour = _shadowColour;
        }
        void draw(Graphics& graphics) {
            D2D1_SIZE_F renderTargetSize = graphics.hwndRenderTarget->GetSize();
            pair<int, int> position = getAbsolutePosition(renderTargetSize);
            D2D_SIZE_F size_as_d2d = D2D_SIZE_F();
            size_as_d2d.width = size.first;
            size_as_d2d.height = size.second;
            D2D1_RECT_F rect = getRect(position, size_as_d2d);
            D2D1_RECT_F shadowRect = getRect({ position.first - 5, position.second - 5 }, size_as_d2d);
            ID2D1SolidColorBrush* theBrush = NULL;
            ID2D1SolidColorBrush* shadowBrush = NULL;
            HRESULT hr = S_OK;
            hr = graphics.hwndRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF(colour[0], colour[1], colour[2], colour[3])),
                &theBrush);
            hr = graphics.hwndRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF(shadowColour[0], shadowColour[1], shadowColour[2], shadowColour[3])),
                &shadowBrush);
            graphics.hwndRenderTarget->DrawText(
                removeTagsBeforePrinting(graphics, message).c_str(),
                message.size(),
                graphics.WriteTextFormats[format],
                rect,
                shadowBrush);
            IDWriteTextLayout * textLayout = NULL;
            graphics.DWriteFactory->CreateTextLayout(removeTagsBeforePrinting(graphics, message).c_str(), message.size(), graphics.WriteTextFormats[format], size_as_d2d.width, size_as_d2d.height, &textLayout);
            D2D1_POINT_2F P; P.x = rect.top; P.y = rect.left;
            Map<string, List<DWRITE_TEXT_RANGE>> subcolours = interpret_subcolours(graphics);
            Map<string, ID2D1SolidColorBrush*> extraBrushes;
            for (auto const& [key, value] : subcolours.internalMap) {
                extraBrushes[key] = NULL;
                graphics.hwndRenderTarget->CreateSolidColorBrush(
                    D2D1::ColorF(D2D1::ColorF(graphics.Colours[key][0], graphics.Colours[key][1], graphics.Colours[key][2], graphics.Colours[key][3])),
                    &extraBrushes[key]);
                for (auto const& range : subcolours[key].internalList) {
                    textLayout->SetDrawingEffect(extraBrushes[key], range);
                }
            }
            graphics.hwndRenderTarget->DrawTextLayout(P, textLayout, theBrush);
            SafeRelease(&theBrush);
            SafeRelease(&shadowBrush);
            SafeRelease(&textLayout);
            for (string x : extraBrushes.getKeys().internalList) {
                SafeRelease(&extraBrushes[x]);
            }
        }
        wstring removeTagsBeforePrinting(Graphics & graphics, wstring input_string) {
            for (int x = 0; x < input_string.size(); x++) {
                if (graphics.colourTagLookupTable.getKeys().contains(input_string[x])) {
                    input_string[x] = wchar_t(32);
                }
            }
            return input_string;
        }
        Map<string, List<DWRITE_TEXT_RANGE>> interpret_subcolours(Graphics & graphics) {
            Map<string, List<DWRITE_TEXT_RANGE>> results;
            for (auto const& colour : graphics.colourTagLookupTable.getKeys().internalList) {
                int current_start = -1;
                bool seekingEnd = false;
                for (int x = 0; x < fullMessage.size(); x++) {
                    if (seekingEnd and current_start != -1 and List<wchar_t>({ wchar_t("."), wchar_t(" "), wchar_t("!"), wchar_t("?"), colour }).contains(fullMessage[x])) {
                        int current_end = x;
                        results[graphics.colourTagLookupTable[colour]].push_back({unsigned(current_start), unsigned(current_end-1)});
                        current_start = -1;
                        seekingEnd = false;
                    }
                    if (fullMessage[x] == colour) {
                        current_start = x;
                        seekingEnd = true;
                    }
                    
                }
                }
            return results;
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
        vector<float> shadowColour = { 0.0,0.0,0.0,1.0 };
        int tagLimit = 10;
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
        idwriteinmemoryfontfileloaders["Centaur"] = NULL;
        hr = DWriteFactory->CreateInMemoryFontFileLoader(&idwriteinmemoryfontfileloaders["Centaur"]);
        hr = DWriteFactory->RegisterFontFileLoader(idwriteinmemoryfontfileloaders["Centaur"]);
        idwritefontfiles["Centaur"] = NULL;
        HINSTANCE hInstance = ::GetModuleHandle(nullptr);
        HRSRC  hFntRes = FindResource(hInstance, MAKEINTRESOURCE(IDF_CENTAUR), L"BINARY");
        HGLOBAL hFntMem = LoadResource(hInstance, hFntRes);
        void* FntData = LockResource(hFntMem);
        DWORD nFonts = 0, len = SizeofResource(hInstance, hFntRes);
        addfontmemresourcefonts["Centaur"] = AddFontMemResourceEx(FntData, len, nullptr, &nFonts);

        idwritefontfiles["Centaur"] = NULL;
        hr = idwriteinmemoryfontfileloaders["Centaur"]->CreateInMemoryFontFileReference(
            DWriteFactory,
            FntData,
            len,
            NULL,
            &idwritefontfiles["Centaur"]);

        hr = DWriteFactory->CreateFontSetBuilder(&fontSetBuilder);
        idwritefontfacereferences["Centaur"] = NULL;
        DWriteFactory->CreateFontFaceReference(idwritefontfiles["Centaur"], 0, DWRITE_FONT_SIMULATIONS_NONE, &idwritefontfacereferences["Centaur"]);
        fontSetBuilder->AddFontFaceReference(idwritefontfacereferences["Centaur"]);

        hr = fontSetBuilder->CreateFontSet(&customFontSet);

        hr = DWriteFactory->CreateFontCollectionFromFontSet(customFontSet, &fontCollection);


        hr = DWriteFactory->CreateTextFormat(L"TestFont1",
            fontCollection,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            40,
            L"",
            &textFormat);
        if (SUCCEEDED(hr)) {
            WriteTextFormats.add({ "TESTFONT1", textFormat });
        }

        filesystem::path path = filesystem::current_path() / "Cursor.cur";
        HCURSOR default_cursor = LoadCursorFromFileA(path.string().c_str());
        Cursors["DEFAULT"] = default_cursor;
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
    void tearDownAllLoadedFonts() {
        for (auto const& [key, val] : addfontmemresourcefonts.internalMap) {
            RemoveFontMemResourceEx(val);
        }
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
    IDWriteFontSetBuilder1* fontSetBuilder;
    IDWriteFontSet* customFontSet;
    IDWriteFontCollection1* fontCollection;
    Map<string, IDWriteTextFormat *> WriteTextFormats;
    Map<string, IDWriteInMemoryFontFileLoader*> idwriteinmemoryfontfileloaders;
    Map<string, IDWriteFontFile*> idwritefontfiles;
    Map<string, HANDLE> addfontmemresourcefonts;
    Map<string, IDWriteFontSetBuilder1*> idwritefontsetbuilders;
    Map<string, IDWriteFontFaceReference*> idwritefontfacereferences;
    IDWriteFactory5* DWriteFactory;
    ID2D1HwndRenderTarget* hwndRenderTarget;
    Map<int, List<Image *>> ImageMap;
    Map<int, Map<string, Text>> TextMap;
    Map<string, HCURSOR> Cursors;
    Map<string, vector<float>> Colours;
    Map<wchar_t, string> colourTagLookupTable;
};
Graphics graphics = Graphics();