/*
 * Mục đích: Xử lý logic game 2048.
 * Chức năng: Xử lý bảng số 4x4, di chuyển ô, gộp số, tính điểm.
 */

#include "logic.h"

int bang_o[4][4];
int diem_so;

/*
 * Mục đích: Đặt lại bảng về trạng thái ban đầu.
 * Logic xử lý: Gán tất cả ô về 0 và đặt điểm về 0.
 */
void khoiTaoLaiBang() {
  // Gán tất cả ô về 0
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      bang_o[i][j] = 0;
    }
  }
  diem_so = 0;
}

/*
 * Mục đích: Sinh số 2 hoặc 4 vào một ô trống ngẫu nhiên.
 * Logic xử lý: Tìm tất cả ô có giá trị 0, chọn ngẫu nhiên 1 ô, gán 2 hoặc 4.
 */
void sinhSoMoi() {
  int hang_o_trong[16];
  int cot_o_trong[16];
  int so_luong_o_trong = 0;

  // Tìm tất cả ô trống trong bảng
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (bang_o[i][j] == 0) {
        hang_o_trong[so_luong_o_trong] = i;
        cot_o_trong[so_luong_o_trong] = j;
        so_luong_o_trong++;
      }
    }
  }

  if (so_luong_o_trong == 0)
    return;

  int vi_tri_ngau_nhien = rand() % so_luong_o_trong;
  int hang_duoc_chon = hang_o_trong[vi_tri_ngau_nhien];
  int cot_duoc_chon = cot_o_trong[vi_tri_ngau_nhien];

  if (rand() % 10 < 9) {
    bang_o[hang_duoc_chon][cot_duoc_chon] = 2;
  } else {
    bang_o[hang_duoc_chon][cot_duoc_chon] = 4;
  }
}

/*
 * Mục đích: Di chuyển tất cả ô sang trái.
 * Logic xử lý: Với mỗi hàng, dồn số khác 0 sang trái, gộp 2 số liền kề bằng
 * nhau, tính điểm.
 */
bool diChuyenTrai() {
  bool co_thay_doi = false;

  // Xử lý từng hàng
  for (int i = 0; i < 4; i++) {
    int hang_tam[4] = {0, 0, 0, 0};
    int vi_tri = 0;

    // Lấy các số khác 0, dồn sang trái
    for (int j = 0; j < 4; j++) {
      if (bang_o[i][j] != 0) {
        hang_tam[vi_tri] = bang_o[i][j];
        vi_tri++;
      }
    }

    // Gộp 2 số liền kề bằng nhau
    for (int j = 0; j < 3; j++) {
      if (hang_tam[j] == hang_tam[j + 1] && hang_tam[j] != 0) {
        hang_tam[j] = hang_tam[j] * 2;
        diem_so = diem_so + hang_tam[j];
        hang_tam[j + 1] = 0;
      }
    }

    int hang_ket_qua[4] = {0, 0, 0, 0};
    vi_tri = 0;

    // Dồn lại sau khi gộp, loại bỏ ô trống
    for (int j = 0; j < 4; j++) {
      if (hang_tam[j] != 0) {
        hang_ket_qua[vi_tri] = hang_tam[j];
        vi_tri++;
      }
    }

    // Cập nhật lại bảng, kiểm tra có thay đổi không
    for (int j = 0; j < 4; j++) {
      if (bang_o[i][j] != hang_ket_qua[j]) {
        co_thay_doi = true;
      }
      bang_o[i][j] = hang_ket_qua[j];
    }
  }
  return co_thay_doi;
}

/*
 * Mục đích: Di chuyển tất cả ô sang phải.
 * Logic xử lý: Với mỗi hàng, dồn số khác 0 sang phải, gộp 2 số liền kề bằng
 * nhau, tính điểm.
 */
bool diChuyenPhai() {
  bool co_thay_doi = false;

  // Xử lý từng hàng
  for (int i = 0; i < 4; i++) {
    int hang_tam[4] = {0, 0, 0, 0};
    int vi_tri = 0;

    // Lấy các số khác 0 từ phải sang trái
    for (int j = 3; j >= 0; j--) {
      if (bang_o[i][j] != 0) {
        hang_tam[vi_tri] = bang_o[i][j];
        vi_tri++;
      }
    }

    // Gộp 2 số liền kề bằng nhau
    for (int j = 0; j < 3; j++) {
      if (hang_tam[j] == hang_tam[j + 1] && hang_tam[j] != 0) {
        hang_tam[j] = hang_tam[j] * 2;
        diem_so = diem_so + hang_tam[j];
        hang_tam[j + 1] = 0;
      }
    }

    int hang_ket_qua[4] = {0, 0, 0, 0};
    vi_tri = 0;

    // Dồn lại sau khi gộp, loại bỏ ô trống
    for (int j = 0; j < 4; j++) {
      if (hang_tam[j] != 0) {
        hang_ket_qua[vi_tri] = hang_tam[j];
        vi_tri++;
      }
    }

    int vi_tri_ghi = 3;

    // Cập nhật lại bảng từ phải sang trái
    for (int j = 0; j < 4; j++) {
      if (bang_o[i][vi_tri_ghi] != hang_ket_qua[j]) {
        co_thay_doi = true;
      }
      bang_o[i][vi_tri_ghi] = hang_ket_qua[j];
      vi_tri_ghi--;
    }
  }
  return co_thay_doi;
}

/*
 * Mục đích: Di chuyển tất cả ô lên trên.
 * Logic xử lý: Với mỗi cột, dồn số khác 0 lên trên, gộp 2 số liền kề bằng nhau,
 * tính điểm.
 */
bool diChuyenLen() {
  bool co_thay_doi = false;

  // Xử lý từng cột
  for (int j = 0; j < 4; j++) {
    int cot_tam[4] = {0, 0, 0, 0};
    int vi_tri = 0;

    // Lấy các số khác 0 từ trên xuống
    for (int i = 0; i < 4; i++) {
      if (bang_o[i][j] != 0) {
        cot_tam[vi_tri] = bang_o[i][j];
        vi_tri++;
      }
    }

    // Gộp 2 số liền kề bằng nhau
    for (int i = 0; i < 3; i++) {
      if (cot_tam[i] == cot_tam[i + 1] && cot_tam[i] != 0) {
        cot_tam[i] = cot_tam[i] * 2;
        diem_so = diem_so + cot_tam[i];
        cot_tam[i + 1] = 0;
      }
    }

    int cot_ket_qua[4] = {0, 0, 0, 0};
    vi_tri = 0;

    // Dồn lại sau khi gộp, loại bỏ ô trống
    for (int i = 0; i < 4; i++) {
      if (cot_tam[i] != 0) {
        cot_ket_qua[vi_tri] = cot_tam[i];
        vi_tri++;
      }
    }

    // Cập nhật lại bảng, kiểm tra có thay đổi không
    for (int i = 0; i < 4; i++) {
      if (bang_o[i][j] != cot_ket_qua[i]) {
        co_thay_doi = true;
      }
      bang_o[i][j] = cot_ket_qua[i];
    }
  }
  return co_thay_doi;
}

/*
 * Mục đích: Di chuyển tất cả ô xuống dưới.
 * Logic xử lý: Với mỗi cột, dồn số khác 0 xuống dưới, gộp 2 số liền kề bằng
 * nhau, tính điểm.
 */
bool diChuyenXuong() {
  bool co_thay_doi = false;

  // Xử lý từng cột
  for (int j = 0; j < 4; j++) {
    int cot_tam[4] = {0, 0, 0, 0};
    int vi_tri = 0;

    // Lấy các số khác 0 từ dưới lên
    for (int i = 3; i >= 0; i--) {
      if (bang_o[i][j] != 0) {
        cot_tam[vi_tri] = bang_o[i][j];
        vi_tri++;
      }
    }

    // Gộp 2 số liền kề bằng nhau
    for (int i = 0; i < 3; i++) {
      if (cot_tam[i] == cot_tam[i + 1] && cot_tam[i] != 0) {
        cot_tam[i] = cot_tam[i] * 2;
        diem_so = diem_so + cot_tam[i];
        cot_tam[i + 1] = 0;
      }
    }

    int cot_ket_qua[4] = {0, 0, 0, 0};
    vi_tri = 0;

    // Dồn lại sau khi gộp, loại bỏ ô trống
    for (int i = 0; i < 4; i++) {
      if (cot_tam[i] != 0) {
        cot_ket_qua[vi_tri] = cot_tam[i];
        vi_tri++;
      }
    }

    int vi_tri_ghi = 3;

    // Cập nhật lại bảng từ dưới lên
    for (int i = 0; i < 4; i++) {
      if (bang_o[vi_tri_ghi][j] != cot_ket_qua[i]) {
        co_thay_doi = true;
      }
      bang_o[vi_tri_ghi][j] = cot_ket_qua[i];
      vi_tri_ghi--;
    }
  }
  return co_thay_doi;
}

/*
 * Mục đích: Kiểm tra còn nước đi hợp lệ hay không.
 * Logic xử lý: Kiểm tra có ô trống không. Nếu không, kiểm tra có 2 ô liền kề
 * bằng nhau không.
 */
bool kiemTraCoTheDiChuyen() {
  // Kiểm tra có ô trống không
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (bang_o[i][j] == 0) {
        return true;
      }
    }
  }

  // Kiểm tra 2 ô liền kề theo hàng ngang có bằng nhau không
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      if (bang_o[i][j] == bang_o[i][j + 1]) {
        return true;
      }
    }
  }

  // Kiểm tra 2 ô liền kề theo cột dọc có bằng nhau không
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 3; i++) {
      if (bang_o[i][j] == bang_o[i + 1][j]) {
        return true;
      }
    }
  }

  return false;
}
