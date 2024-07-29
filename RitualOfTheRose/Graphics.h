#pragma once
#include "framework.h"


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
        SafeRelease(&D2DFactory);
        //SafeRelease(&IWICFactory);
        SafeRelease(&DWriteFactory);
        SafeRelease(&hwndRenderTarget);
        SafeRelease(&BlackBrush);
        SafeRelease(&GridPatternBitmapBrush);
        SafeRelease(&example->texture);
    }
	class Texture {
	public:
        Texture() {}
		Texture(int src) {
			source = src;
		}
		~Texture() {
			SafeRelease(&texture);
		}
		bool isLoaded() {
			return texture != NULL;
		}
        HRESULT loadTexture(Graphics & graphics) {
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
            imageResHandle = FindResource(HINST_THISCOMPONENT, MAKEINTRESOURCE(source), L"PNG");

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

            SafeRelease(&pDecoder);
            SafeRelease(&pSource);
            SafeRelease(&pStream);
            SafeRelease(&pConverter);
            SafeRelease(&pScaler);

            return hr;
        }


        int source;
		ID2D1Bitmap  * texture = NULL;

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
        
            hr = hwndRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Black),
                &BlackBrush
            );
            example = new Texture(IDB_PNG1);
            hr = example->loadTexture(*this);
 
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

            D2D1_SIZE_F renderTargetSize = hwndRenderTarget->GetSize();
            D2D1_SIZE_F size = example->texture->GetSize();
            ID2D1Bitmap * to_draw = example->texture;



            hwndRenderTarget->DrawBitmap(
                example->texture,
                D2D1::RectF(0.0f, 0.0f, size.width, size.height)
            );

            // add code here to draw every image 
        }
        hr = hwndRenderTarget->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET){
            hr = S_OK;
            DiscardDeviceResources();
        }
        return hr;
    }

    HRESULT CreateGridPatternBrush(ID2D1RenderTarget* pRenderTarget, ID2D1BitmapBrush** ppBitmapBrush) {
        HRESULT hr = S_OK;

        // Create a compatible render target.
        ID2D1BitmapRenderTarget* pCompatibleRenderTarget = NULL;
        hr = pRenderTarget->CreateCompatibleRenderTarget(
            D2D1::SizeF(10.0f, 10.0f),
            &pCompatibleRenderTarget
        );
        if (SUCCEEDED(hr))
        {
            // Draw a pattern.
            ID2D1SolidColorBrush* pGridBrush = NULL;
            hr = pCompatibleRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF(0.93f, 0.94f, 0.96f, 1.0f)),
                &pGridBrush
            );
            if (SUCCEEDED(hr))
            {
                pCompatibleRenderTarget->BeginDraw();
                pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, 10.0f, 1.0f), pGridBrush);
                pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.1f, 1.0f, 10.0f), pGridBrush);
                pCompatibleRenderTarget->EndDraw();

                // Retrieve the bitmap from the render target.
                ID2D1Bitmap* pGridBitmap = NULL;
                hr = pCompatibleRenderTarget->GetBitmap(&pGridBitmap);
                if (SUCCEEDED(hr)) {
                    // Choose the tiling mode for the bitmap brush.
                    D2D1_BITMAP_BRUSH_PROPERTIES brushProperties =
                        D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP);

                    // Create the bitmap brush.
                    hr = hwndRenderTarget->CreateBitmapBrush(pGridBitmap, brushProperties, ppBitmapBrush);

                    pGridBitmap->Release();
                }

                pGridBrush->Release();
            }
        }
        return hr;
    }
    void DiscardDeviceResources()
    {
        SafeRelease(&hwndRenderTarget);
    }

    HWND * hwnd;
	ID2D1Factory * D2DFactory;
    IWICImagingFactory * IWICFactory;
    IDWriteFactory* DWriteFactory;
    ID2D1HwndRenderTarget* hwndRenderTarget;
    ID2D1BitmapBrush * GridPatternBitmapBrush;
    ID2D1SolidColorBrush* BlackBrush = NULL;
    Texture * example = NULL;
};