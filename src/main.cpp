/*
 * Mục đích: Hiển thị giao diện game 2048 bằng SDL2.
 * Chức năng: Xử lý sự kiện bàn phím, chuột và vẽ bảng game lên cửa sổ.
 */

#include "logic.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <ctime>
#include <iostream>
#include <string>


using namespace std;

const int KICH_THUOC_O = 100;
const int KHOANG_CACH_O = 15;
const int BAN_KINH_BO_GOC = 6;
const int LE_TRAI = 20;
const int LE_TREN = 140;
const int RONG_CUA_SO = LE_TRAI * 2 + KICH_THUOC_O * 4 + KHOANG_CACH_O * 3 + 12;
const int CAO_CUA_SO = LE_TREN + KICH_THUOC_O * 4 + KHOANG_CACH_O * 5 + 15;

SDL_Rect nut_choi_moi = {RONG_CUA_SO - 150, 80, 130, 40};

bool dang_choi = true;
bool da_thua = false;
int diem_cao_nhat = 0;

/*
 * Mục đích: Tạo màu SDL từ giá trị RGBA.
 * Logic xử lý: Gán R, G, B, A vào struct SDL_Color và trả về.
 */
SDL_Color taoMau(int r, int g, int b, int a) {
  SDL_Color mau;
  mau.r = r;
  mau.g = g;
  mau.b = b;
  mau.a = a;
  return mau;
}

/*
 * Mục đích: Lấy màu nền tương ứng với giá trị ô.
 * Logic xử lý: Dùng switch trả về màu định sẵn cho từng giá trị (0, 2, 4,
 * ..., 2048).
 */
SDL_Color layMauNenO(int gia_tri) {
  switch (gia_tri) {
  case 0:
    return taoMau(43, 44, 60, 255);
  case 2:
    return taoMau(110, 235, 131, 255);
  case 4:
    return taoMau(86, 203, 249, 255);
  case 8:
    return taoMau(38, 185, 154, 255);
  case 16:
    return taoMau(243, 114, 44, 255);
  case 32:
    return taoMau(249, 65, 68, 255);
  case 64:
    return taoMau(240, 45, 100, 255);
  case 128:
    return taoMau(170, 46, 201, 255);
  case 256:
    return taoMau(111, 45, 189, 255);
  case 512:
    return taoMau(22, 138, 173, 255);
  case 1024:
    return taoMau(250, 182, 0, 255);
  case 2048:
    return taoMau(255, 230, 0, 255);
  default:
    return taoMau(54, 55, 83, 255);
  }
}

/*
 * Mục đích: Lấy màu chữ phù hợp với màu nền ô.
 * Logic xử lý: Ô nền sáng (2, 4, 1024, 2048) trả về chữ đen, còn lại trả
 * về chữ trắng.
 */
SDL_Color layMauChu(int gia_tri) {
  if (gia_tri == 2 || gia_tri == 4 || gia_tri == 1024 || gia_tri == 2048) {
    return taoMau(24, 24, 36, 255);
  }
  return taoMau(255, 255, 255, 255);
}

/*
 * Mục đích: Vẽ chuỗi text căn giữa trong một khung chữ nhật.
 * Logic xử lý: Tạo texture từ chuỗi, tính tọa độ căn giữa, vẽ lên
 * renderer.
 */
void veChuCanGiua(SDL_Renderer *renderer, TTF_Font *font, string noi_dung,
                  SDL_Rect khung, SDL_Color mau_chu) {
  if (noi_dung == "")
    return;

  // Bước 1: chuyển chuỗi thành ảnh bitmap
  // .c_str() đổi string sang dạng ký tự mà SDL2 đọc được
  SDL_Surface *surface =
      TTF_RenderUTF8_Blended(font, noi_dung.c_str(), mau_chu);
  if (!surface)
    return;

  // Bước 2: chuyển ảnh bitmap thành texture để SDL2 vẽ lên cửa sổ
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  // surface->w và surface->h là chiều rộng/cao thực tế của chữ sau khi render
  int x = khung.x + (khung.w - surface->w) / 2;
  int y = khung.y + (khung.h - surface->h) / 2;
  SDL_Rect vung_chu = {x, y, surface->w, surface->h};

  // Bước 3: vẽ texture lên cửa sổ
  // NULL: vẽ toàn bộ texture, không cắt bớt
  // &vung_chu: địa chỉ của vùng đích để SDL2 biết vẽ ở đâu
  SDL_RenderCopy(renderer, texture, NULL, &vung_chu);

  // Bước 4: xóa bộ nhớ thủ công vì SDL2 không tự giải phóng
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

/*
 * Mục đích: Vẽ chuỗi text tại tọa độ (x, y).
 * Logic xử lý: Tạo texture từ chuỗi và vẽ lên renderer tại tọa độ chỉ
 * định.
 */
void veChu(SDL_Renderer *renderer, TTF_Font *font, string noi_dung, int x,
           int y, SDL_Color mau_chu) {
  if (noi_dung == "")
    return;

  // Bước 1: chuyển chuỗi thành ảnh bitmap
  SDL_Surface *surface =
      TTF_RenderUTF8_Blended(font, noi_dung.c_str(), mau_chu);
  if (!surface)
    return;

  // Bước 2: chuyển ảnh bitmap thành texture để SDL2 vẽ lên cửa sổ
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  // surface->w và surface->h là chiều rộng/cao thực tế của chữ
  SDL_Rect vung_chu = {x, y, surface->w, surface->h};

  // Bước 3: vẽ texture lên cửa sổ tại tọa độ (x, y)
  // NULL: vẽ toàn bộ texture, không cắt bớt
  // &vung_chu: địa chỉ của vùng đích để SDL2 biết vẽ ở đâu
  SDL_RenderCopy(renderer, texture, NULL, &vung_chu);

  // Bước 4: xóa bộ nhớ thủ công vì SDL2 không tự giải phóng
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

/*
 * Mục đích: Vẽ một ô số trên bảng game.
 * Logic xử lý: Tính tọa độ từ hàng và cột, vẽ hình chữ nhật bo góc với màu
 * nền, vẽ số căn giữa.
 */
void veMotO(SDL_Renderer *renderer, TTF_Font *font_o, int hang, int cot,
            int gia_tri) {
  int x = LE_TRAI + KHOANG_CACH_O + cot * (KICH_THUOC_O + KHOANG_CACH_O);
  int y = LE_TREN + KHOANG_CACH_O + hang * (KICH_THUOC_O + KHOANG_CACH_O);

  SDL_Color mau_nen = layMauNenO(gia_tri);

  roundedBoxRGBA(renderer, x, y, x + KICH_THUOC_O, y + KICH_THUOC_O,
                 BAN_KINH_BO_GOC, mau_nen.r, mau_nen.g, mau_nen.b, mau_nen.a);
  SDL_Rect vung_o = {x, y, KICH_THUOC_O, KICH_THUOC_O};

  if (gia_tri > 0) {
    string chuoi_so = to_string(gia_tri);
    veChuCanGiua(renderer, font_o, chuoi_so, vung_o, layMauChu(gia_tri));
  }
}

/*
 * Mục đích: Vẽ phần tiêu đề gồm tên game, hướng dẫn và nút "Chơi Mới".
 * Logic xử lý: Hiển thị chữ tiêu đề, chữ hướng dẫn, vẽ nút bấm.
 */
void vePhanTieuDe(SDL_Renderer *renderer, TTF_Font *font_tieu_de,
                  TTF_Font *font_nho) {
  veChu(renderer, font_tieu_de, "2048", LE_TRAI, 15,
        taoMau(220, 220, 245, 255));
  veChu(renderer, font_nho, "Gộp các số để tạo ra ô 2048!", LE_TRAI, 92,
        taoMau(160, 160, 180, 255));

  roundedBoxRGBA(
      renderer, nut_choi_moi.x, nut_choi_moi.y, nut_choi_moi.x + nut_choi_moi.w,
      nut_choi_moi.y + nut_choi_moi.h, BAN_KINH_BO_GOC, 116, 82, 255, 255);
  veChuCanGiua(renderer, font_nho, "Chơi Mới", nut_choi_moi,
               taoMau(255, 255, 255, 255));
}

/*
 * Mục đích: Vẽ khung điểm hiện tại và khung kỷ lục.
 * Logic xử lý: Cập nhật điểm cao nhất, vẽ 2 khung hiển thị điểm và kỷ lục.
 */
void veKhungDiemVaKyLuc(SDL_Renderer *renderer, TTF_Font *font_nho) {
  if (diem_so > diem_cao_nhat) {
    diem_cao_nhat = diem_so;
  }

  int chieu_rong_khung = 80;
  SDL_Rect khung_ky_luc = {RONG_CUA_SO - LE_TRAI - chieu_rong_khung, 20,
                           chieu_rong_khung, 55};
  SDL_Rect khung_diem = {RONG_CUA_SO - LE_TRAI - chieu_rong_khung * 2 - 8, 20,
                         chieu_rong_khung, 55};

  roundedBoxRGBA(renderer, khung_diem.x, khung_diem.y,
                 khung_diem.x + khung_diem.w, khung_diem.y + khung_diem.h,
                 BAN_KINH_BO_GOC, 43, 44, 60, 255);
  roundedBoxRGBA(
      renderer, khung_ky_luc.x, khung_ky_luc.y, khung_ky_luc.x + khung_ky_luc.w,
      khung_ky_luc.y + khung_ky_luc.h, BAN_KINH_BO_GOC, 43, 44, 60, 255);

  SDL_Rect vung_tieu_de_diem = {khung_diem.x, khung_diem.y + 5, khung_diem.w,
                                15};
  SDL_Rect vung_tieu_de_ky_luc = {khung_ky_luc.x, khung_ky_luc.y + 5,
                                  khung_ky_luc.w, 15};
  veChuCanGiua(renderer, font_nho, "ĐIỂM", vung_tieu_de_diem,
               taoMau(135, 136, 160, 255));
  veChuCanGiua(renderer, font_nho, "KỶ LỤC", vung_tieu_de_ky_luc,
               taoMau(135, 136, 160, 255));

  SDL_Rect vung_so_diem = {khung_diem.x, khung_diem.y + 25, khung_diem.w, 25};
  SDL_Rect vung_so_ky_luc = {khung_ky_luc.x, khung_ky_luc.y + 25,
                             khung_ky_luc.w, 25};

  veChuCanGiua(renderer, font_nho, to_string(diem_so), vung_so_diem,
               taoMau(255, 255, 255, 255));
  veChuCanGiua(renderer, font_nho, to_string(diem_cao_nhat), vung_so_ky_luc,
               taoMau(255, 255, 255, 255));
}

/*
 * Mục đích: Vẽ nền bảng và 16 ô số 4x4.
 * Logic xử lý: Vẽ khung nền, sau đó vẽ từng ô theo giá trị từ logic game.
 */
void veBangGame(SDL_Renderer *renderer, TTF_Font *font_o) {
  int kich_thuoc_bang = 4 * KICH_THUOC_O + 5 * KHOANG_CACH_O;
  SDL_Rect vung_nen_bang = {LE_TRAI, LE_TREN, kich_thuoc_bang, kich_thuoc_bang};
  roundedBoxRGBA(renderer, vung_nen_bang.x, vung_nen_bang.y,
                 vung_nen_bang.x + vung_nen_bang.w,
                 vung_nen_bang.y + vung_nen_bang.h, BAN_KINH_BO_GOC + 4, 28, 28,
                 42, 255);

  // Vẽ từng ô trong bảng 4x4
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int gia_tri = bang_o[i][j];
      veMotO(renderer, font_o, i, j, gia_tri);
    }
  }
}

/*
 * Mục đích: Vẽ màn hình thua (lớp phủ mờ + chữ "Game Over!").
 * Logic xử lý: Vẽ lớp phủ mờ lên bảng, hiển thị chữ "Game Over!" căn giữa.
 */
void veManHinhThua(SDL_Renderer *renderer, TTF_Font *font_tieu_de) {
  int kich_thuoc_bang = 4 * KICH_THUOC_O + 5 * KHOANG_CACH_O;
  SDL_Rect vung_phu = {LE_TRAI, LE_TREN, kich_thuoc_bang, kich_thuoc_bang};

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  roundedBoxRGBA(renderer, vung_phu.x, vung_phu.y, vung_phu.x + vung_phu.w,
                 vung_phu.y + vung_phu.h, BAN_KINH_BO_GOC + 4, 238, 228, 218,
                 180);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

  SDL_Rect vung_game_over = {LE_TRAI, LE_TREN, kich_thuoc_bang,
                             kich_thuoc_bang};
  veChuCanGiua(renderer, font_tieu_de, "Game Over!", vung_game_over,
               taoMau(255, 50, 50, 255));
}

/*
 * Mục đích: Vẽ toàn bộ giao diện game trong 1 khung hình.
 * Logic xử lý: Xóa màn hình, vẽ tiêu đề, điểm, bảng, và màn hình thua nếu
 * có.
 */
void veGiaoDien(SDL_Renderer *renderer, TTF_Font *font_o,
                TTF_Font *font_tieu_de, TTF_Font *font_nho) {
  // Xóa màn hình cũ
  SDL_SetRenderDrawColor(renderer, 24, 24, 36, 255);
  SDL_RenderClear(renderer);

  // Vẽ từng phần giao diện
  vePhanTieuDe(renderer, font_tieu_de, font_nho);
  veKhungDiemVaKyLuc(renderer, font_nho);
  veBangGame(renderer, font_o);

  // Vẽ màn hình thua nếu đã thua
  if (da_thua) {
    veManHinhThua(renderer, font_tieu_de);
  }

  // Hiển thị khung hình lên cửa sổ
  SDL_RenderPresent(renderer);
}

/*
 * Mục đích: Hàm chính khởi chạy game.
 * Logic xử lý: Khởi tạo SDL, tạo cửa sổ, xử lý sự kiện bàn phím và chuột
 * trong vòng lặp game.
 */
int main(int argc, char *args[]) {
  srand(time(0));
  khoiTaoLaiBang();
  sinhSoMoi();
  sinhSoMoi();

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return 1;
  if (TTF_Init() == -1)
    return 1;

  SDL_Window *cua_so = SDL_CreateWindow(
      "Game 2048 - Nhóm 05", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      RONG_CUA_SO, CAO_CUA_SO, SDL_WINDOW_SHOWN);
  if (!cua_so)
    return 1;

  SDL_Renderer *renderer =
      SDL_CreateRenderer(cua_so, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
    return 1;

  TTF_Font *font_o = TTF_OpenFont("assets/font.ttf", 46);
  TTF_Font *font_tieu_de = TTF_OpenFont("assets/font.ttf", 64);
  TTF_Font *font_nho = TTF_OpenFont("assets/font.ttf", 16);

  if (!font_o || !font_tieu_de || !font_nho)
    return 1;

  SDL_Event su_kien;

  // Vòng lặp chính: xử lý sự kiện và vẽ giao diện cho đến khi thoát
  while (dang_choi) {

    // Xử lý sự kiện từ bàn phím và chuột
    while (SDL_PollEvent(&su_kien) != 0) {
      if (su_kien.type == SDL_QUIT) {
        dang_choi = false;

      } else if (su_kien.type == SDL_MOUSEBUTTONUP) {
        int chuot_x = su_kien.button.x;
        int chuot_y = su_kien.button.y;
        if (chuot_x >= nut_choi_moi.x &&
            chuot_x <= nut_choi_moi.x + nut_choi_moi.w &&
            chuot_y >= nut_choi_moi.y &&
            chuot_y <= nut_choi_moi.y + nut_choi_moi.h) {

          // Tạo dữ liệu cho 2 nút trong hộp thoại xác nhận
          const SDL_MessageBoxButtonData cac_nut[] = {
              {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Hủy"},
              {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Bắt đầu"}};
          const SDL_MessageBoxData hop_thoai = {
              SDL_MESSAGEBOX_WARNING,
              cua_so,
              "Xác nhận",
              "Bảng số hiện tại sẽ bị xóa.\nBạn có chắc chắn muốn bắt đầu trò "
              "chơi mới không?",
              2,
              cac_nut,
              NULL};

          int nut_duoc_chon;
          // Hiển thị hộp thoại xác nhận
          if (SDL_ShowMessageBox(&hop_thoai, &nut_duoc_chon) >= 0) {
            // Nếu chọn Bắt đầu thì khởi tạo lại game
            if (nut_duoc_chon == 1) {
              khoiTaoLaiBang();
              da_thua = false;
              sinhSoMoi();
              sinhSoMoi();
              SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
            }
          }
        }

      } else if (su_kien.type == SDL_KEYDOWN && !da_thua) {
        bool da_di_chuyen = false;

        switch (su_kien.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_w:
          da_di_chuyen = diChuyenLen();
          break;
        case SDLK_DOWN:
        case SDLK_s:
          da_di_chuyen = diChuyenXuong();
          break;
        case SDLK_LEFT:
        case SDLK_a:
          da_di_chuyen = diChuyenTrai();
          break;
        case SDLK_RIGHT:
        case SDLK_d:
          da_di_chuyen = diChuyenPhai();
          break;
        }

        if (da_di_chuyen) {
          sinhSoMoi();
        }

        if (kiemTraCoTheDiChuyen() == false && !da_thua) {
          da_thua = true;

          // Vẽ lại toàn bộ giao diện (có màn hình thua) trước khi bật hộp thoại
          veGiaoDien(renderer, font_o, font_tieu_de, font_nho);

          const SDL_MessageBoxButtonData cac_nut[] = {
              {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Thoát"},
              {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Chơi lại"}};

          string thong_bao =
              "Không còn nước đi nào! Điểm của bạn là: " + to_string(diem_so) +
              "\nBạn có muốn chơi lại không?";
          const SDL_MessageBoxData hop_thoai = {SDL_MESSAGEBOX_INFORMATION,
                                                cua_so,
                                                "Trò chơi kết thúc",
                                                thong_bao.c_str(),
                                                2,
                                                cac_nut,
                                                NULL};

          int nut_duoc_chon;
          // Hiển thị hộp thoại khi thua cuộc
          if (SDL_ShowMessageBox(&hop_thoai, &nut_duoc_chon) >= 0) {
            if (nut_duoc_chon == 1) {
              khoiTaoLaiBang();
              da_thua = false;
              sinhSoMoi();
              sinhSoMoi();
              SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
            } else {
              dang_choi = false; // Thoát nếu không chọn chơi lại
            }
          }
        }
      }
    }

    veGiaoDien(renderer, font_o, font_tieu_de, font_nho);
    SDL_Delay(16);
  }

  if (font_o)
    TTF_CloseFont(font_o);
  if (font_tieu_de)
    TTF_CloseFont(font_tieu_de);
  if (font_nho)
    TTF_CloseFont(font_nho);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(cua_so);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
