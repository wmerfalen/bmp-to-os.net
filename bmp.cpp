#include <iostream>
#include <string>

unsigned char* ReadBMP(const std::string & filename)
{
    int i;
    FILE* f = fopen(filename.c_str(), "rb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];


    int row_padded = (width*3 + 3) & (~3);
    std::vector<unsigned char> data;
	data.reserve(row_padded);
    unsigned char tmp;

    for(int i = 0; i < height; i++)
    {
        fread((unsigned char*)&data[0], sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;
/*
 * The color of pixel (i, j) is stored at: 
	data[j * width + i], 
	data[j * width + i + 1] 
	data[j * width + i + 2]
*/
            cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
        }
    }

    fclose(f);
    return data;
}
