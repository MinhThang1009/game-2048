/*
 * Mục đích: Sinh mã hệ thống test tự động cho ứng dụng.
 * Chức năng: Đọc lưới và rà soát tìm ra sai xót của thuật toán.
 */

#include <iostream>
#include "../src/logic.h"

using namespace std;

/*
 * Mục đích: Ghép số thông báo mảng hiển thị thành đồ thị trên console.
 * Cách xử lý: Đọc tọa độ hai chiều của ma trận rồi gọi câu lệnh cout xuất số rải dài cho dễ đối chiếu so sánh bằng mắt nhanh.
 */
void inBang(Game2048 &game) {
    // Quét dòng trên dọc dưới rải giá trị
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << game.layGiaTriO(i, j) << "\t";
        }
        cout << endl;
    }
    cout << "-------------" << endl;
}

/*
 * Mục đích: Thử nghiệm đánh giá độ bền thuật toán thả số mới.
 * Cách xử lý: Tìm mảng ván mới số không, thả lệnh gọi random 2 lần sinh số 2 hoặc 4. Đếm thông số xem nếu không đúng rơi ra hai mảng ô có số khác không thì đánh rớt bug.
 */
bool testKichBanGopHaiSo2() {
    Game2048 gameTest;
    
    cout << "Ma tran hien trang truoc cap so:" << endl;
    inBang(gameTest);
    
    gameTest.sinhSoMoi();
    gameTest.sinhSoMoi();
    
    cout << "Ma tran sau thao tac sinh 2 so ngau nhien:" << endl;
    inBang(gameTest);
    
    int demGach = 0;
    
    // Đếm tính thông số lượng ô có dữ liệu lớn hơn 0
    for (int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(gameTest.layGiaTriO(i, j) != 0){
                demGach++;
            }
        }
    }
    
    if (demGach == 2) {
        return true; 
    }
    return false;
}

/*
 * Mục đích: Chạy máy dồn bài toán tự kiểm định test xem lỗi không.
 * Cách xử lý: Gọi hàm kiểm tra ngẫu nhiên và dội ngược đánh giá màn màu là kết luận dội qua hàm Pass hay Failed gửi cho lập trình.
 */
int main() {
    cout << "Tien trinh khoi chay Unit Test:" << endl;
    
    bool ket_qua = testKichBanGopHaiSo2();
    if (ket_qua) {
        cout << "Test 1 - Kiem tra ham sinh so ngau nhien: PASSED." << endl;
    } else {
        cout << "Test 1 - Kiem tra ham sinh so ngau nhien: FAILED." << endl;
    }

    return 0;
}
