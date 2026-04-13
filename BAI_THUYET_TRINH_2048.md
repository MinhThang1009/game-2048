# BÀI THUYẾT TRÌNH: DỰ ÁN GAME 2048 (C++ & SDL2)

Tài liệu này được soạn thảo chi tiết để giúp bạn hiểu bản chất từng dòng code trong dự án, cách cấu trúc chương trình, và đặc biệt là cách đối phó với những câu hỏi vặn của thầy cô.

---

## 1. TỔNG QUAN KIẾN TRÚC DỰ ÁN
Mã nguồn được thiết kế theo nguyên tắc **Tách biệt mối quan tâm (Separation of Concerns)**.
- **`logic.cpp` & `logic.h` (Model):** Lớp não bộ. Xử lý thuật toán, bảng điều khiển 4x4, quy luật thắng/thua, tính điểm. Hoàn toàn không biết trên màn hình đang vẽ cái gì.
- **`main.cpp` (View & Controller):** Lớp giao diện. Dùng thư viện SDL2 để vẽ ra cửa sổ, bắt sự kiện phím, chuột. Nó dùng kết quả của não bộ để quyết định vẽ số mấy vào hệ tọa độ nào.
- **`Makefile`:** Kịch bản cấu hình, giúp gộp các file code rời rạc lại thành 1 file chạy `.exe` duy nhất cho nhiều hệ điều hành chỉ bằng 1 dòng lệnh `make`.

> **💡 Giáo viên hỏi vặn:** "Tại sao em không viết gộp hết vào 1 file `main.cpp` cho dễ nhìn mà phải tách ra?"
> **👉 Trả lời:** "Dạ để Code có tính linh hoạt và dễ bảo trì (Clean Code). Nếu em muốn đổi giao diện từ cửa sổ ứng dụng sang chơi trên màn hình đen trắng Console (Terminal), em chỉ việc vứt file `main.cpp` đi, thay khối hiển thị mới vào và giữ nguyên mảng não bộ bên `logic.cpp` không phải viết lại luật game."

---

## 2. HIỂU SÂU VỀ BỘ NÃO: `logic.cpp`

### 2.1. Cấu trúc cấu thành
- `int bang_o[4][4]`: Mảng 2 chiều 4x4 đóng vai trò là sân chơi. `0` là ô trống.
- `int diem_so`: Tổng điểm.

### 2.2. Hàm `sinhSoMoi()`
Vào mỗi lúc mới mở game hoặc di chuyển thành công, game cần đẻ thêm số `2` hoặc `4`.
* **Cách làm:**
  1. Ghi lại tọa độ vị trí của TẤT CẢ các ô đang có số `0` bỏ vào mảng `hang_o_trong` và `cot_o_trong`.
  2. Bốc thăm bằng logic: `rand() % so_luong_o_trong` để chọn 1 vị trí ngẫu nhiên.
  3. Giá trị gán vào là `2` hay `4` cũng được bốc thăm bằng tỷ lệ 50/50 qua đoạn `rand() % 2`.

> **💡 Giáo viên hỏi vặn:** "Hàm `rand()` sinh số ngẫu nhiên, vậy nếu tắt app đi rồi mở lại, cái số ngẫu nhiên đó có bị lặp lại chu kỳ không?"
> **👉 Trả lời:** "Nếu không can thiệp thì sẽ lặp. Do đó, ngay đầu file `main.cpp` em đã gọi `srand(time(0));` - dùng múi giờ hiện tại để seed (gieo hạt) cho hàm Random, đảm bảo mỗi lần người chơi bật game là một trải nghiệm hoàn toàn ngẫu nhiên."

### 2.3. Hạch tâm thuật toán: Hàm Di Chuyển (Ví dụ `diChuyenTrai()`)
Khi bấm phím sang **TRÁI**, game sẽ đem từng hàng ngang ra xử lý. Mỗi hàng ngang quy trình có **3 BƯỚC CHUẨN**:
1. **Dồn khít:** Duyệt qua hàng, bốc các số KHÁC 0 xếp dồn về phía trái, tạo ra mảng `hang_tam`. (VD: `[2, 0, 2, 0]` => `[2, 2, 0, 0]`).
2. **Gộp số giống nhau:** Nếu có 2 ô đứng cạnh nhau giống nhau, ô bên trái nhân 2, ô bên phải tự hủy thành số 0. (Từ mảng tam `[2, 2, 0, 0]` => `[4, 0, 0, 0]`). Ở bước này ta sẽ cộng thêm vào biến `diem_so`.
3. **Dồn khít lần 2:** Vì bước gộp lại đẻ ra lỗ hổng, số lại phải được lùa dồn tiếp 1 lần nữa để vá lỗ hổng đó.
Cuối cùng, hàm so sánh lại toàn bộ với mảng ban đầu, nếu CÓ THAY ĐỔI -> hàm trả về cờ `true` biểu thị di chuyển hợp lệ.

> **💡 Giáo viên hỏi vặn:** "Tại sao bắt buộc hàm trả về `true`/`false` mà không dùng `void`?"
> **👉 Trả lời:** "Dạ nếu người ta đã sát lề trái, mảng không thể xê dịch mà cứ cấm đầu nhấn Mũi tên Trái, thì game không được phép đẻ thêm số mới. Cờ `true` thông báo cho mảng biết có gộp/xê dịch thành công thì mới được gọi hàm `sinhSoMoi()`."

### 2.4. Hàm Báo Tử: `kiemTraCoTheDiChuyen()`
Kiểm tra xem khi nào Trò chơi Kết thúc. Trò chơi CHƯA TẮT THỞ khi:
- Bảng còn chừa ô trống (`0`).
- Hoặc, bảng đã đầy 16 số, nhưng có 2 số đứng liền cạnh nhau bằng nhau theo hàng dọc hoặc hàng ngang (còn có cơ hội gộp ăn điểm).

---

## 3. LỚP ÁO GIAO DIỆN (`main.cpp`)

Phần này dùng **SDL2** tạo ra 60 khung hình/giây.

### 3.1. Hiển thị UI (`veGiaoDien()`)
- Sử dụng phép toán logic hệ tọa độ `x`, `y`. Ví dụ: `x = LE_TRAI + KHOANG_CACH_O + cot * (KICH_THUOC_O)`.
- **Cấu trúc tô màu:** Code tạo sẵn hàm `layMauNenO()`. Viết cấu trúc `switch(gia_tri) ... case 8 ... case 16` để đưa ra chuẩn RGB mã màu cho từng loại số.
- Về render text (font tiếng Việt), dùng `TTF_RenderUTF8_Blended()`.

> **💡 Giáo viên hỏi vặn:** "Em có biết rò rỉ bộ nhớ (Memory Leak) là gì không? Thư viện SDL có cơ chế chống rắc rối này không?"
> **👉 Trả lời:** "Dạ có. Mỗi khi convert 1 text thành Texture để vẽ theo quy trình của SDL, hệ thống sẽ cấp phát RAM máy tính. SDL bản quyền viết bằng C nên không có 'Môi trường tự gom rác' (Garbage Collector). Nên trong hàm `veChuCanGiua()`, sau khi vẽ xong em luôn phải tự gọi hàm `SDL_FreeSurface(surface);` và `SDL_DestroyTexture(texture);` để dọn RAM."

### 3.2. Vòng lặp siêu kết nối (Game Loop)
Trong `main()`, có vòng `while (dang_choi)` tạo nhịp đập cho màn hình (60 FPS):
1. **Lắng nghe sự kiện:** `SDL_PollEvent` bắt hành động Mũi tên ấn xuống (`SDL_KEYDOWN`) gán với hàm di chuyển, chuột click (`SDL_MOUSEBUTTONUP`) gán với nút restart.
2. Kiểm tra cờ GameOver, nếu chết thì show cái Popup Alert bằng cờ `SDL_ShowMessageBox()` để hỏi Chơi lại hay không.
3. Kêu thằng Họa Sĩ vẽ lại toàn bộ biến số trên bảng lúc này ra màn hình qua `veGiaoDien()`.
4. Gọi `SDL_Delay(16);` giúp máy tính nghỉ ngơi (chỉ tối đa render được cỡ 60 FPS), tránh ăn lạm RAM, và CPU quá cao.

> **💡 Giáo viên hỏi vặn:** "Làm sao màn hình tạo ra được lớp phủ sương mờ mờ ở nền lúc thua?"
> **👉 Trả lời:** "Trong thuật toán đồ họa dùng RGBA, tham số A (Alpha) quản lý độ mờ đục. Bình thường là 255. Để tạo mờ thua cuộc em vẽ một hình chữ nhật đè lên bảng với thông số A = 180, và quan trọng nhất phải bật chế độ trộn màu cho cọ vẽ bằng lệnh `SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);` thì thư viện mới cho phép trộn màu với mảng nền bị đè lên dưới áo."

---

## 4. QUÁ TRÌNH BUILD DÙNG `Makefile`
Do đặc thù Windows chạy MSys2 rất phiền. `Makefile` trong dự án này đã dùng lệnh rẽ nhánh điều kiện `ifdef MSYSTEM`:
- Nếu đúng là khởi tạo qua MSys2, các Linker flags bao gồm `-lSDL2main -lSDL2 ...`.
- Sử dụng lệnh `g++` gom tất cả file mã nguồn lại ra file đuôi `.exe`.

> **💡 Giáo viên hỏi vặn:** "Thấy em để cái copy Font ở Makefile làm gì thế?"
> **👉 Trả lời:** "Khi game `.exe` chạy, nó sẽ tìm tệp `font.ttf` ở thư mục hiện tại theo đường dẫn quan hệ ngang hàng. Nếu file Build được đặt ở trong folder `/build`, em cần tự động luân chuyển sao chép file `assets/font.ttf` thảy vào chung rổ `/build` thì app lúc chạy lên mới lấy được đường dẫn font tiếng việt, chứ không nó sẽ văng Lỗi Không tìm thấy null pointer font."

## 🚀 LỜI KHUYÊN KHI THUYẾT TRÌNH:
1. Đừng run sợ, Cấu trúc dự án bạn có sự phân chia View-Model rạch ròi, đây là điểm cộng lớn nhất hãy nói nhiều về cái này.
2. Thầy cô C/C++ thường hỏi rất nhiều về **Quản lý RAM** và **Xử lý Mảng 2 Chiều**. Hãy học thuộc lòng cái bước *Dồn - Gộp - Dồn* của phần (2.3) của tài liệu này để phản biện tốt nhất.
3. Chúc bạn đạt điểm A!
