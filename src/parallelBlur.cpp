#include <iostream>
#include <vector>
#include <chrono>
#include <thread>


struct Color{
    int red;
    int green;
    int blue;

    Color(): red(0),green(0),blue(0) {}
    Color(int r,int g,int b):red(r),green(g),blue(b){}

};


//отдельная часть размытия по рядам
void Blur(const std::vector<std::vector<Color>>& img, std::vector<std::vector<Color>>& new_img, int start, int end) {
    int row = img.size();
    int str = img[0].size();
    int razm = 5; // размер размытия

    for (int i = start; i < end; i++) {
        for (int j = 0; j < str; j++) {
            int count = 0;
            int count_red = 0;
            int count_green = 0;
            int count_blue = 0;
            for (int ki = -razm / 2; ki <= razm / 2; ki++) {
                for (int kj = -razm / 2; kj <= razm / 2; kj++) {
                    int n_row = i + ki;
                    int n_col = j + kj;
                    if (n_row >= 0 && n_col >= 0 && n_row < row && n_col < str) {
                        count_red += img[n_row][n_col].red;
                        count_green += img[n_row][n_col].green;
                        count_blue += img[n_row][n_col].blue;
                        count++;
                    }
                }
            }
            new_img[i][j].red = count_red / count;
            new_img[i][j].green = count_green / count;
            new_img[i][j].blue = count_blue / count;
        }
    }
}

std::vector<std::vector<Color>> parallelBlur(const std::vector<std::vector<Color>>& img, int count_threads) {
    int row = img.size();
    int str = img[0].size();
    std::vector<std::vector<Color>> new_img(row, std::vector<Color>(str));

    //потоки
    std::vector<std::thread> threads;
    int rows_thread = row / count_threads;

    for (int i = 0; i < count_threads; i++) {
        int startRow = i * rows_thread;
        int endRow=0;
        if (i==count_threads-1){
            endRow=row;
        } else {
            endRow=startRow+rows_thread;
        }
        threads.push_back(std::thread(Blur, std::cref(img), std::ref(new_img), startRow, endRow));
    }

    //ожидание завершения всех потоков
    for (auto& th : threads) {
        th.join();
    }

    return new_img;
}



int main() {

    std::vector<std::vector<Color>> image(100, std::vector <Color> (100)) ; //100*100
    for (int i=0;i<30;i++){
        for (int j=0;j<20;j++){
            image[i][j] = Color (255, 0, 0);
        } 
    }

    for (int i=30;i<70;i++){
        for (int j=20;j<50;j++){
            image[i][j] = Color (255, 0, 0);
        } 
    }
    for (int i=70;i<100;i++){
        for (int j=50;j<100;j++){
            image[i][j] = Color (255, 255, 0);
        }
    }
    //2 потока
    auto start_img=std::chrono::high_resolution_clock::now();
    auto blur_img = parallelBlur(image,2);
    auto end_img=std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> diff_img=end_img-start_img;
    std::cout<<"img_time(2 threads):"<<diff_img.count()<<std::endl;

    //5 потоков
    auto start1_img=std::chrono::high_resolution_clock::now();
    auto blur1_img = parallelBlur(image,5);
    auto end1_img=std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> diff2_img=end1_img-start1_img;
    std::cout<<"img_time(5 threads):"<<diff2_img.count()<<std::endl;


    return 0;
}