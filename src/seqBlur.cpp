#include <iostream>
#include <vector>
#include <chrono>

struct Color{
    int red;
    int green;
    int blue;

    Color(): red(0),green(0),blue(0) {}
    Color(int r,int g,int b):red(r),green(g),blue(b){}

};

std::vector<std::vector<Color>> sequentialBlur (std::vector<std::vector<Color>>& img){
    int row=img.size();
    int str=img[0].size();
    std::vector<std::vector<Color>> new_img (row, std::vector <Color>(str));

    int razm=img[0].size(); //размытие 5*5
    for (int i=0;i<row;i++){
        for (int j=0;j<str;j++){
            int count=0;
            int count_red=0;
            int count_green=0;
            int count_blue=0;
            for (int ki=-razm/2;ki<=razm/2;ki++){
                for (int kj=-razm/2;kj<=razm/2;kj++){
                    int n_row=i+ki;
                    int n_str=j+kj;
                    if (n_row>0 && n_str>0 && n_row<row && n_str<str){
                        count_red += img[n_row][n_str].red;
                        count_green += img[n_row][n_str].green;
                        count_blue += img[n_row][n_str].blue;
                        count++;
                    }
                }
            }
            new_img[i][j].red = count_red/count;
            new_img[i][j].green = count_green/count;
            new_img[i][j].blue = count_blue/count;

        }
    }
    
    return new_img;

}

int main() {
    // 5*5
    std::vector<std::vector<Color>> image1 = {
        {Color(255, 0, 0), Color(255, 0, 0), Color(255, 0, 0), Color(255, 0, 0), Color(255, 0, 0)},
        {Color(255, 0, 0), Color(0, 255, 0), Color(0, 255, 0), Color(255, 0, 0), Color(255, 0, 0)},
        {Color(255, 0, 0), Color(0, 255, 0), Color(0, 255, 0), Color(255, 0, 0), Color(255, 0, 0)},
        {Color(255, 0, 0), Color(0, 255, 0), Color(0, 255, 0), Color(255, 0, 0), Color(255, 0, 0)},
        {Color(255, 0, 0), Color(255, 0, 0), Color(255, 0, 0), Color(255, 0, 0), Color(255, 0, 0)}
    };
    auto start_img1=std::chrono::high_resolution_clock::now();
    auto blur_img1 = sequentialBlur(image1);
    auto end_img1=std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> diff_img1=end_img1-start_img1;
    std::cout<<"img1_time:"<<diff_img1.count()<<std::endl;

    for (const auto& row : blur_img1) {
        for (const auto& pxl : row) {
            std::cout << "(" << (int)pxl.red << ", " << (int)pxl.green << ", " << (int)pxl.blue << ") ";
        }
        std::cout << std::endl;
    }

    std::vector<std::vector<Color>> image2(100, std::vector <Color> (100)) ; //100*100
    for (int i=0;i<30;i++){
        for (int j=0;j<20;j++){
            image2[i][j] = Color (255, 0, 0);
        } 
    }

    for (int i=30;i<70;i++){
        for (int j=20;j<50;j++){
            image2[i][j] = Color (255, 0, 0);
        } 
    }
    for (int i=70;i<100;i++){
        for (int j=50;j<100;j++){
            image2[i][j] = Color (255, 255, 0);
        }
    }
    auto start_img2=std::chrono::high_resolution_clock::now();
    auto blur_img2 = sequentialBlur(image2);
    auto end_img2=std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> diff_img2=end_img2-start_img2;
    std::cout<<"img2_time:"<<diff_img2.count()<<std::endl;

    return 0;
}