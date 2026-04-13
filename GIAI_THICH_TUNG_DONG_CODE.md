# GIẢI THÍCH CHI TIẾT TỪNG DÒNG CODE C/C++ DÀNH CHO NGƯỜI CHƯA RÀNH

Tài liệu này được anh viết riêng cho em, phân tích giống như một người gia sư ngồi cạnh chỉ từng dòng code một. Đọc kỹ file này em sẽ tự tin trả lời bất kỳ câu hỏi nào về "dòng code này có ý nghĩa gì".

---

## 🚀 1. KIẾN THỨC C++ CƠ BẢN DÙNG TRONG BÀI (Ôn nhanh)
- **Mảng 2 chiều (`int bang_o[4][4]`):** Hãy tưởng tượng như 1 bảng Excel có 4 hàng ngang và 4 cột dọc.
- **Biến toàn cục (Global Variable):** Khai báo ở trên cùng (sau các chữ `#include`). Mọi hàm ở dưới đều có thể đọc/sửa biến này. Ở đây là mảng `bang_o` và biến `diem_so`.
- **Dấu sao `*` (Con trỏ - Pointer):** Ví dụ `SDL_Window *cua_so`. Em hiểu đơn giản là thay vì copy cái cửa sổ to đùng bưng đi khắp nơi (rất nặng máy), nó trỏ tới cái địa chỉ nơi lưu cái cửa sổ đó (như bảng tên đường). 
- **`#include "logic.h"`:** Nối file code hiện tại với file `logic.h`.

---

## 🧠 2. MỔ XẺ NÃO BỘ – FILE `logic.cpp`

### 🔹 Hàm khởi tạo ván mới
```cpp
void khoiTaoLaiBang() {
    // Vòng lặp lồng nhau: i duyệt 4 hàng ngang, j duyệt 4 cột dọc
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            bang_o[i][j] = 0; // Gán toàn bộ 16 ô vuông trên bảng thành số 0 (nghĩa là ô trống)
        }
    }
    diem_so = 0; // Xóa sạch điểm số về 0.
}
```

### 🔹 Hàm đẻ số ngẫu nhiên: `sinhSoMoi()`
```cpp
void sinhSoMoi() {
    int hang_o_trong[16];   // Mảng chứa tối đa 16 vị trí hàng của các ô đang trống
    int cot_o_trong[16];    // Mảng chứa tọa độ cột tương ứng đồ nghề ở bên trên
    int so_luong_o_trong = 0; // Lúc đầu đương nhiên đếm = 0

    // BƯỚC 1: Tìm xem trên bảng chỗ nào bằng 0 (nghĩa là trống)
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (bang_o[i][j] == 0) { // Nếu tìm thấy...
                hang_o_trong[so_luong_o_trong] = i; // Lưu lại tọa độ Hàng
                cot_o_trong[so_luong_o_trong] = j;  // Lưu lại tọa độ Cột
                so_luong_o_trong++; // Tăng biến đếm lên, (tìm đc ô trống thứ 1, thứ 2,...)
            }
        }
    }

    if (so_luong_o_trong == 0) return; // Nếu bảng chật cứng => không làm gì cả, văng ra khỏi hàm luôn!

    // BƯỚC 2: Bốc thăm ngẫu nhiên 1 ô trống trong cái danh sách vừa nhặt được
    int vi_tri_ngau_nhien = rand() % so_luong_o_trong; 
    // Mẹo C++: Dùng phép chia lấy dư (%) sẽ ép con số quái quỷ rand() đẻ ra gói gọn rớt vào từ 0 đến (số lượng - 1). 

    int hang_duoc_chon = hang_o_trong[vi_tri_ngau_nhien];
    int cot_duoc_chon = cot_o_trong[vi_tri_ngau_nhien];

    // BƯỚC 3: Điền số 2 hoặc 4 (Tỷ lệ 50-50 bằng cách chia 2 lấy số dư: dư 0 hoặc dư 1)
    if (rand() % 2 == 0) {
        bang_o[hang_duoc_chon][cot_duoc_chon] = 2;
    } else {
        bang_o[hang_duoc_chon][cot_duoc_chon] = 4;
    }
}
```

### 🔹 CỰC KỲ QUAN TRỌNG: Hàm di chuyển (sẽ lấy `diChuyenTrai()` làm form chuẩn)
*Cách vận hành: Duyệt từ trên xuống dưới, bế TỪNG HÀNG RA xếp riêng ra 1 mảng tạm 1 chiều tên là `hang_tam [4]`. Quậy nát mảng tạm đó cho nó ép qua Trái, rồi mới bốc dán ngược lại về bảng.*

```cpp
bool diChuyenTrai() {
    bool co_thay_doi = false; // Cái cờ (flag), đánh dấu xem đi xong bảng có xê dịch xíu nào ko.

    for (int i = 0; i < 4; i++) { // Quét 4 hàng ngang
        int hang_tam[4] = {0, 0, 0, 0}; // Lập mảng đóng thế, chừa sẵn 4 lỗ hổng
        int vi_tri = 0;

        // --- GIAI ĐOẠN 1: DỒN SỐ CÓ NGHĨA SANG GÓC TRÁI MẢNG TẠM ---
        for (int j = 0; j < 4; j++) {
            if (bang_o[i][j] != 0) { // Nếu ô có số... => Bốc nó nhét sang bên trái mảng tạm (vị trí 0 rồi dần dần lên vị trí 1, 2)
                hang_tam[vi_tri] = bang_o[i][j];
                vi_tri++; // Dịch con trỏ lên, chừa chỗ nhét số tiếp theo.
            }
        }
        // Lúc này, giả sử hàng gốc là [2, 0, 2, 0], qua mảng tạm nó thành [2, 2, 0, 0] rỗng đằng sau rồi.

        // --- GIAI ĐOẠN 2: THU THẬP - GỘP 2 THẰNG KỀ CẠNH GIỐNG NHAU ---
        for (int j = 0; j < 3; j++) { // Chỉ chạy xét j: 0, 1, 2. Tại sao? Vì thằng thứ 3 nó đứng cuối chót rồi, lấy đâu ra thằng thứ 4 (j+1) mà so?
            if (hang_tam[j] == hang_tam[j + 1] && hang_tam[j] != 0) { // Thấy 2 thằng liên cạnh bằng nhau
                hang_tam[j] = hang_tam[j] * 2; // Nhồi cục thịt vào người thằng bên TRÁI. (x2)
                diem_so = diem_so + hang_tam[j]; // Đồng thời báo lên server cộng điểm vô
                hang_tam[j + 1] = 0; // Cho số bên cạnh tan biến, thay bằng con số 0.
            }
        }
        // Giả sử mảng tam trên đoạn này sẽ thành: [4, 0, 0, 0]

        // --- GIAI ĐOẠN 3: DỒN LẠI LẦN CUỐI ĐỂ LẤP HỎNG Ở BƯỚC GỘP LÀM LỚT LÃI KHE ---
        int hang_ket_qua[4] = {0, 0, 0, 0}; 
        vi_tri = 0;
        for (int j = 0; j < 4; j++) {
            if (hang_tam[j] != 0) { 
                hang_ket_qua[vi_tri] = hang_tam[j];
                vi_tri++; 
            }
        }
        // Đã hoàn hảo [4,0,0,0], lùa lại vào bảng chính

        // --- GIAI ĐOẠN 4: OP VÀO BẢNG CHÍNH VÀ KIỂM TRA XÊ DỊCH ---
        for (int j = 0; j < 4; j++) {
            if (bang_o[i][j] != hang_ket_qua[j]) { // Xem bảng cũ mày có khác kết quả mới lấy không?
                co_thay_doi = true;    // Có khác mảy may là bật cờ "CÓ THAY ĐỔI" lên ngay.
            }
            bang_o[i][j] = hang_ket_qua[j];
        }
    }
    return co_thay_doi; // Gửi lại cái Cờ về hàm main
}
```

---

## 🎨 3. LỚP ÁO VẼ (GIAO DIỆN) – FILE `main.cpp`
Đây là xài thư viện. Không mang tính thuật toán khó, chỉ là học vẹt "luật" của bộ SDL C++. 
- `cua_so` (Window): Chính là cái khung app em kéo chuột vi vu được.
- `renderer` (Cây cọ vẽ): Công nhân nhận nhiệm vụ bôi sơn RGB vào bên trong Khung bằng lệnh.

### 🔹 Vòng Lặp Vạn Vật (Game Loop)
Game không giống Word, Excel có chữ là gõ. Game giống 1 bộ tivi chiếu ảnh lộn nhào 60 lần 1 giây nên bắt buộc phải có Vòng lặp `while`.
```cpp
// Vòng lặp này nếu chưa chết thì không cho thoát (luôn luôn = True)
while (dang_choi) { 
    // Bước 1:SDL_PollEvent => Nó đi hóng biến y như 1 đứa nhiều chuyện. Chờ bàn phím gõ xuống hay chuột nhắm vào để báo lên.
    while (SDL_PollEvent(&su_kien) != 0) { 
      // Em nhấn Nút Tắt Khung (Chữ X màu đỏ trên góc Màn Hình)
      if (su_kien.type == SDL_QUIT) {
        dang_choi = false; // Ngắt điện Game
      } 
      else if (su_kien.type == SDL_KEYDOWN && !da_thua) { // Gõ Phím đi em và Game chưa GameOver
        bool da_di_chuyen = false;

        switch (su_kien.key.keysym.sym) { // .sym là dịch phím ấn đó ra mã chữ xem 
        case SDLK_UP:   da_di_chuyen = diChuyenLen(); break;
        // Bắt đầu nhồi Não Bộ chạy. Cái hàm diChuyen...() sẽ quậy tung cái mảng chứa bảng của mình
        }

        if (da_di_chuyen) { // Nhờ có cái cờ True (Có xê dịch nghen) nên tao mới chịu đẻ số mới
          sinhSoMoi();
        }

        // Kiểm tra xem Não báo điềm gì. Nếu Não báo liệt sờ lết rồi -> Phất cờ thua.
        if (kiemTraCoTheDiChuyen() == false && !da_thua) {
           da_thua = true;
           // Đoạn bên dưới hàm tự gọi SDL_ShowMessageBox hiện popup "Chơi Lại - Hoặc cút"
        }
      }
    }

    // Bước 3 Quan Trọng: Vẽ đi thằng họa sĩ!! 
    // Em cứ việc chơi, xê dịch, cờ bạc bên trên. Xong hết em bắt thằng họa sĩ nó update đè lên tảng tranh cái kết quả hiện tại.
    veGiaoDien(renderer, font_o, font_tieu_de, font_nho);
    
    // Bước 4: 1 giây tivi quét 60 lần (1000 mili giây chia 60 = cở 16)
    SDL_Delay(16); // Ép chạy với tốc độ 60 FrameFPS, khỏi điên chip.
}
```

### 🔹 Ý nghĩa của đoạn Dọn dẹp (`SDL_Destroy` và `TTF_CloseFont`)
Cuối file `main.cpp`. C++ là ngôn ngữ "Mày mượn đồ thì tự mày phải xách đi dẹp", không giống Python dẹp giùm.
Anh mượn cái Window, cái renderer (Cọ vẽ), cái Phông chữ -> Chơi Game xong anh phải:
```cpp
  TTF_CloseFont(font_o);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(cua_so);
```
Mục đích: Không cho dính rác trong RAM cái máy tính của em sau khi tắt app. 

---
### TỔNG KẾT BÍ KÍP
Nếu lên trình bày mà run rẩy, hãy mang file này ra xem. Nếu thầy cứ nhất quyết vặn "Dòng code này (`...`) em copy ở đâu mà xài, tính năng nó là gì?". Hãy nhớ:
1. `bang_o[i][j]` : Nó lấy cái tòa nhà có tọa độ tầng ngang `i`, phòng xép `j`.
2. `SDL_RenderCopy` / `roundedBoxRGBA`: Lệnh dùng cọ vẽ hình, không liên quan tới thuật toán, chỉ là xài tool, dồn tọa độ X, Y có sẵn ở thư viện đưa.
3. Kẻ đứng sau cái trò di chuyển các số của `2048` chính là kĩ năng **Lập Mảng Tạm Phụ** (mảng rỗng, nhồi bọn nó qua để khỏi cắn đè, rồi hùa tụi nó về lại).
