/*
 * Mục đích: Hiển thị giao diện game 2048 bằng SDL2.
 * Chức năng: Xử lý sự kiện bàn phím, chuột và vẽ bảng game lên cửa sổ.
 */

#include "logic.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

using namespace std;

const int KICH_THUOC_O = 100;
const int KHOANG_CACH_O = 15;
const int BORDER_RADIUS = 6;
const int LUI_LE_TRAI = 20;
const int LUI_LE_TREN = 140;
const int CHIEU_RONG_CUA_SO = LUI_LE_TRAI * 2 + KICH_THUOC_O * 4 + KHOANG_CACH_O * 3 + 12;
const int CHIEU_CAO_CUA_SO = LUI_LE_TREN + KICH_THUOC_O * 4 + KHOANG_CACH_O * 5 + 15;

SDL_Rect nutChoiMoi = {CHIEU_RONG_CUA_SO - 150, 80, 130, 40};

bool dang_choi = true;
bool tran_thua = false;
int diem_cao_nhat = 0;

/*
 * Mục đích: Tạo màu SDL từ giá trị RGBA.
 * Cách xử lý: Gán R, G, B, A vào struct SDL_Color và trả về.
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
 * Cách xử lý: Dùng switch trả về màu định sẵn cho từng giá trị (0, 2, 4, ..., 2048).
 */
SDL_Color layMauNenO(int gia_tri) {
  switch (gia_tri) {
  case 0:    return taoMau(43, 44, 60, 255);
  case 2:    return taoMau(110, 235, 131, 255);
  case 4:    return taoMau(86, 203, 249, 255);
  case 8:    return taoMau(38, 185, 154, 255);
  case 16:   return taoMau(243, 114, 44, 255);
  case 32:   return taoMau(249, 65, 68, 255);
  case 64:   return taoMau(240, 45, 100, 255);
  case 128:  return taoMau(170, 46, 201, 255);
  case 256:  return taoMau(111, 45, 189, 255);
  case 512:  return taoMau(22, 138, 173, 255);
  case 1024: return taoMau(250, 182, 0, 255);
  case 2048: return taoMau(255, 230, 0, 255);
  default:   return taoMau(54, 55, 83, 255);
  }
}

/*
 * Mục đích: Lấy màu chữ phù hợp với màu nền ô.
 * Cách xử lý: Ô nền sáng (2, 4, 1024, 2048) trả về chữ đen, còn lại trả về chữ trắng.
 */
SDL_Color layMauChu(int gia_tri) {
  if (gia_tri == 2 || gia_tri == 4 || gia_tri == 1024 || gia_tri == 2048) {
    return taoMau(24, 24, 36, 255);
  }
  return taoMau(255, 255, 255, 255);
}

/*
 * Mục đích: Vẽ chuỗi text căn giữa trong một khung chữ nhật.
 * Cách xử lý: Tạo texture từ chuỗi, tính tọa độ căn giữa, vẽ lên renderer.
 */
void veChuCanGiua(SDL_Renderer *renderer, TTF_Font *font, string chuoi_chu, SDL_Rect khung, SDL_Color mau_chu) {
  if (chuoi_chu == "") return;

  SDL_Surface *tmp_surface = TTF_RenderUTF8_Blended(font, chuoi_chu.c_str(), mau_chu);
  if (!tmp_surface) return;

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);

  int tx = khung.x + (khung.w - tmp_surface->w) / 2;
  int ty = khung.y + (khung.h - tmp_surface->h) / 2;
  SDL_Rect o_chu = {tx, ty, tmp_surface->w, tmp_surface->h};

  SDL_RenderCopy(renderer, texture, NULL, &o_chu);
  SDL_FreeSurface(tmp_surface);
  SDL_DestroyTexture(texture);
}

/*
 * Mục đích: Vẽ chuỗi text tại tọa độ (x, y).
 * Cách xử lý: Tạo texture từ chuỗi và vẽ lên renderer tại tọa độ chỉ định.
 */
void veChu(SDL_Renderer *renderer, TTF_Font *font, string chuoi_chu, int x, int y, SDL_Color mau_chu) {
  if (chuoi_chu == "") return;

  SDL_Surface *tmp_surface = TTF_RenderUTF8_Blended(font, chuoi_chu.c_str(), mau_chu);
  if (!tmp_surface) return;

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
  SDL_Rect khung_chu = {x, y, tmp_surface->w, tmp_surface->h};
  SDL_RenderCopy(renderer, texture, NULL, &khung_chu);
  SDL_FreeSurface(tmp_surface);
  SDL_DestroyTexture(texture);
}

/*
 * Mục đích: Vẽ một ô vuông bo góc có số bên trong.
 * Cách xử lý: Tính tọa độ từ hàng và cột, vẽ hình chữ nhật bo góc với màu nền, vẽ số căn giữa.
 */
void veOVuong(SDL_Renderer *renderer, TTF_Font *font, int hang, int cot, int gia_tri) {
  int x = LUI_LE_TRAI + KHOANG_CACH_O + cot * (KICH_THUOC_O + KHOANG_CACH_O);
  int y = LUI_LE_TREN + KHOANG_CACH_O + hang * (KICH_THUOC_O + KHOANG_CACH_O);

  SDL_Color mauNen = layMauNenO(gia_tri);

  roundedBoxRGBA(renderer, x, y, x + KICH_THUOC_O, y + KICH_THUOC_O, BORDER_RADIUS, mauNen.r, mauNen.g, mauNen.b, mauNen.a);
  SDL_Rect oVuongRect = {x, y, KICH_THUOC_O, KICH_THUOC_O};

  if (gia_tri > 0) {
    string chuoi_so = to_string(gia_tri);
    veChuCanGiua(renderer, font, chuoi_so, oVuongRect, layMauChu(gia_tri));
  }
}

/*
 * Mục đích: Vẽ toàn bộ giao diện game lên màn hình.
 * Cách xử lý: Xóa màn hình, vẽ tiêu đề, điểm, nút chơi mới, bảng 4x4, và màn hình thua nếu có.
 */
void veGiaoDienVaoManHinh(SDL_Renderer *renderer, TTF_Font *font_so, TTF_Font *font_to, TTF_Font *font_nho, Game2048 &game) {
  int diemHienTai = game.layDiemSo();
  if (diemHienTai > diem_cao_nhat) {
    diem_cao_nhat = diemHienTai;
  }

  SDL_SetRenderDrawColor(renderer, 24, 24, 36, 255);
  SDL_RenderClear(renderer);

  veChu(renderer, font_to, "2048", LUI_LE_TRAI, 15, taoMau(220, 220, 245, 255));

  int rong_o_diem = 80;
  SDL_Rect oBest = {CHIEU_RONG_CUA_SO - LUI_LE_TRAI - rong_o_diem, 20, rong_o_diem, 55};
  SDL_Rect oScore = {CHIEU_RONG_CUA_SO - LUI_LE_TRAI - rong_o_diem * 2 - 8, 20, rong_o_diem, 55};

  roundedBoxRGBA(renderer, oScore.x, oScore.y, oScore.x + oScore.w, oScore.y + oScore.h, BORDER_RADIUS, 43, 44, 60, 255);
  roundedBoxRGBA(renderer, oBest.x, oBest.y, oBest.x + oBest.w, oBest.y + oBest.h, BORDER_RADIUS, 43, 44, 60, 255);

  SDL_Rect lblScore = {oScore.x, oScore.y + 5, oScore.w, 15};
  SDL_Rect lblBest = {oBest.x, oBest.y + 5, oBest.w, 15};
  veChuCanGiua(renderer, font_nho, "ĐIỂM", lblScore, taoMau(135, 136, 160, 255));
  veChuCanGiua(renderer, font_nho, "KỶ LỤC", lblBest, taoMau(135, 136, 160, 255));

  SDL_Rect valScore = {oScore.x, oScore.y + 25, oScore.w, 25};
  SDL_Rect valBest = {oBest.x, oBest.y + 25, oBest.w, 25};

  veChuCanGiua(renderer, font_nho, to_string(diemHienTai), valScore, taoMau(255, 255, 255, 255));
  veChuCanGiua(renderer, font_nho, to_string(diem_cao_nhat), valBest, taoMau(255, 255, 255, 255));

  veChu(renderer, font_nho, "Gộp các số để tạo ra ô 2048!", LUI_LE_TRAI, 92, taoMau(160, 160, 180, 255));

  roundedBoxRGBA(renderer, nutChoiMoi.x, nutChoiMoi.y, nutChoiMoi.x + nutChoiMoi.w, nutChoiMoi.y + nutChoiMoi.h, BORDER_RADIUS, 116, 82, 255, 255);
  veChuCanGiua(renderer, font_nho, "Chơi Mới", nutChoiMoi, taoMau(255, 255, 255, 255));

  int rong_bang = 4 * KICH_THUOC_O + 5 * KHOANG_CACH_O;
  SDL_Rect tamNenBang = {LUI_LE_TRAI, LUI_LE_TREN, rong_bang, rong_bang};
  roundedBoxRGBA(renderer, tamNenBang.x, tamNenBang.y, tamNenBang.x + tamNenBang.w, tamNenBang.y + tamNenBang.h, BORDER_RADIUS + 4, 28, 28, 42, 255);

  // Vẽ 16 ô trong bảng 4x4
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int giaTriO = game.layGiaTriO(i, j); 
      veOVuong(renderer, font_so, i, j, giaTriO);
    }
  }

  if (tran_thua) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    roundedBoxRGBA(renderer, tamNenBang.x, tamNenBang.y, tamNenBang.x + tamNenBang.w, tamNenBang.y + tamNenBang.h, BORDER_RADIUS + 4, 238, 228, 218, 180);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_Rect khungThua = {LUI_LE_TRAI, LUI_LE_TREN, rong_bang, rong_bang};
    veChuCanGiua(renderer, font_to, "Game Over!", khungThua, taoMau(255, 50, 50, 255));
  }

  SDL_RenderPresent(renderer);
}

/*
 * Mục đích: Hàm chính khởi chạy game.
 * Cách xử lý: Khởi tạo SDL, tạo cửa sổ, xử lý sự kiện bàn phím và chuột trong vòng lặp game.
 */
int main(int argc, char *args[]) {
  srand(time(0));

  Game2048 gameMoi; 
  gameMoi.sinhSoMoi();
  gameMoi.sinhSoMoi();

  if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
  if (TTF_Init() == -1) return 1;

  SDL_Window *window = SDL_CreateWindow("Game 2048 - Nhóm 05", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CHIEU_RONG_CUA_SO, CHIEU_CAO_CUA_SO, SDL_WINDOW_SHOWN);
  if (!window) return 1;

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  TTF_Font *font_so = TTF_OpenFont("C:\\Windows\\Fonts\\arialbd.ttf", 46);
  TTF_Font *font_to = TTF_OpenFont("C:\\Windows\\Fonts\\arialbd.ttf", 64);
  TTF_Font *font_nho = TTF_OpenFont("C:\\Windows\\Fonts\\arialbd.ttf", 16);

  if (!font_so || !font_to || !font_nho) return 1;

  SDL_Event su_kien;

  // Vòng lặp chính của game
  while (dang_choi) {
    
    // Xử lý sự kiện từ bàn phím và chuột
    while (SDL_PollEvent(&su_kien) != 0) { 
      if (su_kien.type == SDL_QUIT) {
        dang_choi = false; 

      } else if (su_kien.type == SDL_MOUSEBUTTONDOWN) {
        int mx = su_kien.button.x;
        int my = su_kien.button.y;
        if (mx >= nutChoiMoi.x && mx <= nutChoiMoi.x + nutChoiMoi.w &&
            my >= nutChoiMoi.y && my <= nutChoiMoi.y + nutChoiMoi.h) {
          gameMoi.datLaiTieude(); 
          tran_thua = false;
          gameMoi.sinhSoMoi(); 
          gameMoi.sinhSoMoi();
        }

      } else if (su_kien.type == SDL_KEYDOWN && !tran_thua) {
        bool di_chuyen_thanh_cong = false; 

        switch (su_kien.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_w:
          di_chuyen_thanh_cong = gameMoi.diChuyenLen();
          break;
        case SDLK_DOWN:
        case SDLK_s:
          di_chuyen_thanh_cong = gameMoi.diChuyenXuong();
          break;
        case SDLK_LEFT:
        case SDLK_a:
          di_chuyen_thanh_cong = gameMoi.diChuyenTrai();
          break;
        case SDLK_RIGHT:
        case SDLK_d:
          di_chuyen_thanh_cong = gameMoi.diChuyenPhai();
          break;
        }

        if (di_chuyen_thanh_cong) {
          gameMoi.sinhSoMoi();
        }

        if (gameMoi.kiemTraCoTheDiChuyen() == false) { 
          tran_thua = true; 
        }
      }
    }

    veGiaoDienVaoManHinh(renderer, font_so, font_to, font_nho, gameMoi);
    SDL_Delay(16); 
  }

  if (font_so) TTF_CloseFont(font_so);
  if (font_to) TTF_CloseFont(font_to);
  if (font_nho) TTF_CloseFont(font_nho);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
