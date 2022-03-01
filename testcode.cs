private void 
int[] histogram = new int[256];

int width = mGrayPixels.Width;
int height = mGrayPixels.Height;

for (int y = 0; y < height; ++y)
{
    for (int x = 0; x < width; ++x)
    {
        byte gray = mGrayPixels[y, x];
        histogram[gray]++;
    }
}

// cache
int[] accumHistogram = new int[256];

accumHistogram[0] = histogram[0];
for (int i = 1; i < histogram.length; ++i)
{
    accumHistogram[i] = accumHistogram[i - 1]; 
}

int[] equalizedHistogram = new int[256];
for (int i = 1; i < histogram.length; ++i)
{
    equalizedHistogram[i] = Math.Round(accumHistogram * 255 / (width * height));
}

for (int y = 0; y < height; ++y)
{
    for (int x = 0; x < width; ++x)
    {
        byte gray = mGrayPixels[y, x];
        mGrayPixels[y, x] = equalizedHistogram[gray];
    }
}