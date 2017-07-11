#include <iostream>
#include <string>
#include <vector>

int width = 0;
int height = 0;
std::vector<unsigned int> ReadBMP(const std::string & filename)
{
    int i;
    FILE* f = fopen(filename.c_str(), "rb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    width = *(int*)&info[18];
    height = *(int*)&info[22];

    int row_padded = (width*3 + 3) & (~3);
    std::vector<unsigned int> data;
	data.reserve(row_padded * height);
    unsigned char tmp;

    std::vector<unsigned char> single_row; 
    single_row.reserve(row_padded);
    for(int i = 0; i < height; i++)
    {
        std::fill(single_row.begin(),single_row.end(),0);
        fread((unsigned char*)&single_row[0], sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = single_row[j];
            single_row[j] = single_row[j+2];
            single_row[j+2] = tmp;
            data.push_back((unsigned int)single_row[j] + \
                    (unsigned int)single_row[j+1] + \
                    (unsigned int)single_row[j+2]);
/*
 * The color of pixel (i, j) is stored at: 
 * data[j * width + i], 
 * data[j * width + i + 1] 
 * data[j * width + i + 2]
*/
            std::cout << "R: "<< (int)single_row[j] << " G: " << (int)single_row[j+1];
            std::cout << " B: " << (int)single_row[j+2]<< "\n";
        }
    }
    fclose(f);
    return data;
}

int usage(char ** argv){
    std::cerr << "Usage: " << argv[0] << " <file.bmp> <threshold> <lower_threshold> <flip:0|1>\n";
    return 1;
}

/*
struct note {
    unsigned char** scale;
    unsigned char* start;
    unsigned short octave;
    unsigned char* current;
    note() : octave(7),start("G#"),scale({"B","A#","A","G#","G","F#","F","E","D#","D","C#","C"}){
        current = indexof(start);
    }
    unsigned int indexof(unsigned char* c){
        for(unsigned i=0;i < sizeof(scale);i++){
            if(strcmp(scale[i],c) == 0){
                return i;
            }
        }
        return (unsigned int)"lolwtf";
    }
    operator++(){
        if(strcmp(scale[current],"C") ==0){
            current = 0;
        }else{
            ++current;
        }
    }
};
*/

int main(int argc,char** argv){
    if(argc != 5){
        return usage(argv);
    }
    auto data = ReadBMP(std::string(argv[1]));
    int threshold = atoi(argv[2]);
    int lower_threshold = atoi(argv[3]);
    bool flip = atoi(argv[4]) == 1 ? true : false;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width;x++){
            /* We want 0,0 */
            auto pixel = data[y * width + x];
            if(flip){
                pixel = data[x * height + y];
            }
            if(pixel < threshold && pixel > lower_threshold){
                std::cout << "*";
            }else if(pixel < threshold && pixel < lower_threshold && pixel > lower_threshold/2){
                std::cout << ".";
            }else{
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    return 0;
}
