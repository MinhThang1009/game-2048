/*
 * Mục đích: Xử lý logic game 2048.
 * Chức năng: Quản lý bảng số 4x4, di chuyển ô, gộp số, tính điểm.
 */

#include "logic.h"

/*
 * Mục đích: Khởi tạo game.
 * Cách xử lý: Gọi datLaiTieude() để đặt bảng về trạng thái ban đầu.
 */
Game2048::Game2048() {
    datLaiTieude();
}

/*
 * Mục đích: Đặt lại bảng về trạng thái ban đầu.
 * Cách xử lý: Gán tất cả ô về 0 và đặt điểm về 0.
 */
void Game2048::datLaiTieude() {
    // Gán tất cả ô về 0
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            bang[i][j] = 0;
        }
    }
    diem_so = 0;
}

/*
 * Mục đích: Lấy điểm hiện tại.
 * Cách xử lý: Trả về biến diem_so.
 */
int Game2048::layDiemSo() {
    return diem_so;
}

/*
 * Mục đích: Lấy giá trị một ô trong bảng.
 * Cách xử lý: Trả về bang[hang][cot].
 */
int Game2048::layGiaTriO(int hang, int cot) {
    return bang[hang][cot];
}

/*
 * Mục đích: Sinh số 2 hoặc 4 vào một ô trống ngẫu nhiên.
 * Cách xử lý: Tìm tất cả ô có giá trị 0, chọn ngẫu nhiên 1 ô, gán 2 hoặc 4.
 */
void Game2048::sinhSoMoi() {
    int o_trong_hang[16];
    int o_trong_cot[16];
    int so_luong_o_trong = 0;

    // Tìm tất cả ô trống trong bảng
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (bang[i][j] == 0) { 
                o_trong_hang[so_luong_o_trong] = i; 
                o_trong_cot[so_luong_o_trong] = j;  
                so_luong_o_trong++;                 
            }
        }
    }

    if (so_luong_o_trong == 0) return;

    int lua_chon_ngau_nhien = rand() % so_luong_o_trong;
    int hang_chieu_dinh = o_trong_hang[lua_chon_ngau_nhien];
    int cot_chieu_dinh = o_trong_cot[lua_chon_ngau_nhien];

    if (rand() % 2 == 0) {
        bang[hang_chieu_dinh][cot_chieu_dinh] = 2;
    } else {
        bang[hang_chieu_dinh][cot_chieu_dinh] = 4;
    }
}

/*
 * Mục đích: Di chuyển tất cả ô sang trái.
 * Cách xử lý: Với mỗi hàng, dồn số khác 0 sang trái, gộp 2 số liền kề bằng nhau, tính điểm.
 */
bool Game2048::diChuyenTrai() {
    bool co_o_nao_di_chuyen = false;

    // Xử lý từng hàng
    for (int i = 0; i < 4; i++) {
        int hang_tam_thoi[4] = {0, 0, 0, 0};
        int vi_tri_tam = 0;

        // Lấy các số khác 0, dồn sang trái
        for (int j = 0; j < 4; j++) {
            if (bang[i][j] != 0) {
                hang_tam_thoi[vi_tri_tam] = bang[i][j];
                vi_tri_tam++;
            }
        }

        // Gộp 2 số liền kề bằng nhau
        for (int j = 0; j < 3; j++) {
            if (hang_tam_thoi[j] == hang_tam_thoi[j + 1] && hang_tam_thoi[j] != 0) {
                hang_tam_thoi[j] = hang_tam_thoi[j] * 2; 
                diem_so = diem_so + hang_tam_thoi[j];    
                hang_tam_thoi[j + 1] = 0;                
            }
        }

        int hang_ket_qua[4] = {0, 0, 0, 0};
        vi_tri_tam = 0;
        
        // Loại bỏ các ô giá trị 0 sau khi gộp
        for (int j = 0; j < 4; j++) {
            if (hang_tam_thoi[j] != 0) {
                hang_ket_qua[vi_tri_tam] = hang_tam_thoi[j];
                vi_tri_tam++;
            }
        }

        // Cập nhật lại bảng, kiểm tra có thay đổi không
        for (int j = 0; j < 4; j++) {
            if (bang[i][j] != hang_ket_qua[j]) { 
                co_o_nao_di_chuyen = true; 
            }
            bang[i][j] = hang_ket_qua[j];
        }
    }
    return co_o_nao_di_chuyen;
}

/*
 * Mục đích: Di chuyển tất cả ô sang phải.
 * Cách xử lý: Với mỗi hàng, dồn số khác 0 sang phải, gộp 2 số liền kề bằng nhau, tính điểm.
 */
bool Game2048::diChuyenPhai() {
    bool co_o_nao_di_chuyen = false;

    // Xử lý từng hàng
    for (int i = 0; i < 4; i++) {
        int hang_tam_thoi[4] = {0, 0, 0, 0};
        int vi_tri_tam = 0;
        
        // Lấy các số khác 0 từ phải sang trái
        for (int j = 3; j >= 0; j--) { 
            if (bang[i][j] != 0) {
                hang_tam_thoi[vi_tri_tam] = bang[i][j];
                vi_tri_tam++;
            }
        }

        // Gộp 2 số liền kề bằng nhau
        for (int j = 0; j < 3; j++) {
            if (hang_tam_thoi[j] == hang_tam_thoi[j + 1] && hang_tam_thoi[j] != 0) {
                hang_tam_thoi[j] = hang_tam_thoi[j] * 2;
                diem_so = diem_so + hang_tam_thoi[j];
                hang_tam_thoi[j + 1] = 0;
            }
        }

        int hang_ket_qua[4] = {0, 0, 0, 0};
        vi_tri_tam = 0;
        
        // Loại bỏ các ô giá trị 0 sau khi gộp
        for (int j = 0; j < 4; j++) {
            if (hang_tam_thoi[j] != 0) {
                hang_ket_qua[vi_tri_tam] = hang_tam_thoi[j];
                vi_tri_tam++;
            }
        }

        int chi_so_boc_xep = 3; 
        
        // Cập nhật lại bảng từ phải sang trái
        for (int j = 0; j < 4; j++) {
            if (bang[i][chi_so_boc_xep] != hang_ket_qua[j]) {
                co_o_nao_di_chuyen = true;
            }
            bang[i][chi_so_boc_xep] = hang_ket_qua[j];
            chi_so_boc_xep--;
        }
    }
    return co_o_nao_di_chuyen;
}

/*
 * Mục đích: Di chuyển tất cả ô lên trên.
 * Cách xử lý: Với mỗi cột, dồn số khác 0 lên trên, gộp 2 số liền kề bằng nhau, tính điểm.
 */
bool Game2048::diChuyenLen() {
    bool co_o_nao_di_chuyen = false;

    // Xử lý từng cột
    for (int j = 0; j < 4; j++) {
        int cot_tam_thoi[4] = {0, 0, 0, 0};
        int vi_tri_tam = 0;
        
        // Lấy các số khác 0 từ trên xuống
        for (int i = 0; i < 4; i++) {
            if (bang[i][j] != 0) {
                cot_tam_thoi[vi_tri_tam] = bang[i][j];
                vi_tri_tam++;
            }
        }

        // Gộp 2 số liền kề bằng nhau
        for (int i = 0; i < 3; i++) {
            if (cot_tam_thoi[i] == cot_tam_thoi[i + 1] && cot_tam_thoi[i] != 0) {
                cot_tam_thoi[i] = cot_tam_thoi[i] * 2;
                diem_so = diem_so + cot_tam_thoi[i];
                cot_tam_thoi[i + 1] = 0;
            }
        }

        int cot_ket_qua[4] = {0, 0, 0, 0};
        vi_tri_tam = 0;
        
        // Loại bỏ các ô giá trị 0 sau khi gộp
        for (int i = 0; i < 4; i++) {
            if (cot_tam_thoi[i] != 0) {
                cot_ket_qua[vi_tri_tam] = cot_tam_thoi[i];
                vi_tri_tam++;
            }
        }

        // Cập nhật lại bảng, kiểm tra có thay đổi không
        for (int i = 0; i < 4; i++) {
            if (bang[i][j] != cot_ket_qua[i]) {
                co_o_nao_di_chuyen = true;
            }
            bang[i][j] = cot_ket_qua[i];
        }
    }
    return co_o_nao_di_chuyen;
}

/*
 * Mục đích: Di chuyển tất cả ô xuống dưới.
 * Cách xử lý: Với mỗi cột, dồn số khác 0 xuống dưới, gộp 2 số liền kề bằng nhau, tính điểm.
 */
bool Game2048::diChuyenXuong() {
    bool co_o_nao_di_chuyen = false;

    // Xử lý từng cột
    for (int j = 0; j < 4; j++) {
        int cot_tam_thoi[4] = {0, 0, 0, 0};
        int vi_tri_tam = 0;
        
        // Lấy các số khác 0 từ dưới lên
        for (int i = 3; i >= 0; i--) {
            if (bang[i][j] != 0) {
                cot_tam_thoi[vi_tri_tam] = bang[i][j];
                vi_tri_tam++;
            }
        }

        // Gộp 2 số liền kề bằng nhau
        for (int i = 0; i < 3; i++) {
            if (cot_tam_thoi[i] == cot_tam_thoi[i + 1] && cot_tam_thoi[i] != 0) {
                cot_tam_thoi[i] = cot_tam_thoi[i] * 2;
                diem_so = diem_so + cot_tam_thoi[i];
                cot_tam_thoi[i + 1] = 0;
            }
        }

        int cot_ket_qua[4] = {0, 0, 0, 0};
        vi_tri_tam = 0;
        
        // Loại bỏ các ô giá trị 0 sau khi gộp
        for (int i = 0; i < 4; i++) {
            if (cot_tam_thoi[i] != 0) {
                cot_ket_qua[vi_tri_tam] = cot_tam_thoi[i];
                vi_tri_tam++;
            }
        }

        int chi_so_boc_xep = 3;
        
        // Cập nhật lại bảng từ dưới lên
        for (int i = 0; i < 4; i++) {
            if (bang[chi_so_boc_xep][j] != cot_ket_qua[i]) {
                co_o_nao_di_chuyen = true;
            }
            bang[chi_so_boc_xep][j] = cot_ket_qua[i];
            chi_so_boc_xep--;
        }
    }
    return co_o_nao_di_chuyen;
}

/*
 * Mục đích: Kiểm tra còn nước đi hợp lệ hay không.
 * Cách xử lý: Kiểm tra có ô trống không. Nếu không, kiểm tra có 2 ô liền kề bằng nhau không.
 */
bool Game2048::kiemTraCoTheDiChuyen() {
    // Kiểm tra có ô trống không
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (bang[i][j] == 0) {
                return true; 
            }
        }
    }

    // Kiểm tra 2 ô liền kề theo hàng ngang có bằng nhau không
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) { 
            if (bang[i][j] == bang[i][j + 1]) {
                return true; 
            }
        }
    }

    // Kiểm tra 2 ô liền kề theo cột dọc có bằng nhau không
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 3; i++) {
            if (bang[i][j] == bang[i + 1][j]) {
                return true; 
            }
        }
    }

    return false;
}
