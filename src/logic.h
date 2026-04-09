/*
 * Mục đích: Khai báo lớp Game2048.
 * Chức năng: Định nghĩa thuộc tính và phương thức của class Game2048.
 */

#ifndef LOGIC_H
#define LOGIC_H

#include <iostream>
#include <cstdlib>
#include <ctime>

class Game2048 {
private:
    int bang_o[4][4];
    int diem_so;

public:
    Game2048();
    void khoiTaoLaiBang();
    
    int layDiemSo();
    int layGiaTriTaiO(int hang, int cot);
    
    void sinhSoMoi();
    
    bool diChuyenTrai();
    bool diChuyenPhai();
    bool diChuyenLen();
    bool diChuyenXuong();
    
    bool kiemTraCoTheDiChuyen();
};

#endif
