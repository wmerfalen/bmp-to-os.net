#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <array>

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
        }
    }
    fclose(f);
    return data;
}

int usage(char ** argv){
    std::cerr << "Usage: " << argv[0] << " <file.bmp> <threshold> <lower_threshold> <flip:0|1>\n";
    return 1;
}

struct note {
    std::array<const char*,12> scale;
    const char* start;
    short octave;
    const char* current;
    note() : octave(7),start("G#"){
        scale = {"B","A#","A","G#","G","F#","F","E","D#","D","C#","C"};
        current = scale[indexof(start)];
    }
    unsigned int indexof(const char* c){
        for(unsigned i=0;i < sizeof(scale);i++){
            if(strcmp(scale[i],c) == 0){
                return i;
            }
        }
        throw "wtf";
    }
    note& operator++(int){
        if(strcmp(current,"C") ==0){
            current = scale[0];
            --octave;
        }else{
            current = scale[indexof(current)+1];
        }
        return *this;
    }
};

int main(int argc,char** argv){
    if(argc != 5){
        return usage(argv);
    }
    auto data = ReadBMP(std::string(argv[1]));
    int threshold = atoi(argv[2]);
    int lower_threshold = atoi(argv[3]);
    bool flip = atoi(argv[4]) == 1 ? true : false;
    std::cout << "$.post('/app/api/save.php',{";
    std::cout << "title:'Untitled',basedon:0,data:'{\"instruments\":{\"0\":{\"volume\":1,\"delay\":false,\"reverb\":false}}}|110|";//0 A5 1 0;1 G#5 1 0;6 F5 1 0;;
    /**
     * Format of note data is:
     * X NF 1 0 
     * Where X is the x coordinate on the sequencer,
     * and N is the note, and F is the octave
     */
    note current_note;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width;x++){
            /* We want 0,0 */
            auto pixel = data[y * width + x];
            if(flip){
                pixel = data[x * height + y];
            }
            if(pixel < threshold && pixel > lower_threshold){
                std::cout << x << " " << current_note.current << current_note.octave << " 1 0;";
                std::cerr << "*";
            }else{
                std::cerr << " ";
                continue;
            }
        }
        current_note++;
        std::cerr << "\n";
    }
    std::cout << "'}).done(function(msg){location.href=msg.split('>')[1].split('<')[0];});";
    return 0;
}
