typedef char cell_t;

int CalcGame(cell_t * current,
             cell_t * up,
             cell_t * down,
             cell_t * result,
             size_t height,
             size_t width)
{
    if (current == nullptr ||
        up      == nullptr ||
        down    == nullptr ||
        result  == nullptr
        )
        return -1;
    if (width == 0 || height == 0)
        return -2;
    if (height == 1)
    {
        for (size_t i0 = 0; i0 < width; ++i0)
        {
            size_t i1 = (i0 + 1) % width;
            size_t i2 = (i0 + 2) % width;
            cell_t nc = current[i0] +
                        current[i2] +
                        up[i0] +
                        up[i1] +
                        up[i2] +
                        down[i0] +
                        down[i1] +
                        down[i2];
            result[i1] = (current[i1]) ? (nc == 2 || nc == 3) : nc == 3;
        }
        return 0;
    }
    for (size_t i0 = 0; i0 < width; ++i0)
    {
        size_t i1 = (i0 + 1) % width;
        size_t i2 = (i0 + 2) % width;
        cell_t nc = current[i0] +
                    current[i2] +
                    up[i0] +
                    up[i1] +
                    up[i2] +
                    current[i0 + width] +
                    current[i1 + width] +
                    current[i2 + width];
        result[i1] = (current[i1]) ? (nc == 2 || nc == 3) : nc == 3;
    }
    for (size_t j = 1; j < height - 1; ++j)
    {
        for (size_t i0 = 0; i0 < width; ++i0)
        {
            size_t i1 = (i0 + 1) % width;
            size_t i2 = (i0 + 2) % width;
            cell_t nc = current[i0 + width * j] +
                        current[i2 + width * j] +
                        current[i0 + width * (j - 1)] +
                        current[i1 + width * (j - 1)] +
                        current[i2 + width * (j - 1)] +
                        current[i0 + width * (j + 1)] +
                        current[i1 + width * (j + 1)] +
                        current[i2 + width * (j + 1)];
            result[i1 + width * j] = (current[i1 + width * j]) ? (nc == 2 || nc == 3) : nc == 3;
        }
    }
    for (size_t i0 = 0; i0 < width; ++i0)
    {
        size_t i1 = (i0 + 1) % width;
        size_t i2 = (i0 + 2) % width;
        cell_t nc = current[i0 + (height - 1) * width] +
                    current[i2 + (height - 1) * width] +
                    down[i0] +
                    down[i1] +
                    down[i2] +
                    current[i0 + (height - 2) * width] +
                    current[i1 + (height - 2) * width] +
                    current[i2 + (height - 2) * width];
        result[i1 + width * (height - 1)] = (current[i1 + width * (height - 1)]) ? (nc == 2 || nc == 3) : nc == 3;
    }
    return 0;
}