#include <stdio.h>
#include <string.h>

#define MAX_VE 50

// Định nghĩa cấu trúc VeSuKien
typedef struct {
    int maVe;
    char tenSuKien[50];
    char loaiVe[20];
    float giaGoc;
    int isEarlyBird;
    int daCheckIn;
} VeSuKien;

// Hàm tìm kiếm vé theo mã (Hàm hạt nhân)
int timVeTheoMa(VeSuKien ds[], int soLuong, int maCanTim) {
    for (int i = 0; i < soLuong; i++) {
        if (ds[i].maVe == maCanTim) {
            return i;
        }
    }
    return -1;
}

// Hàm tính giá vé thực tế sau khi áp dụng Early Bird
float tinhGiaThanhToan(VeSuKien v) {
    if (v.isEarlyBird == 1) {
        return v.giaGoc * 0.85f; // Giảm 15%
    }
    return v.giaGoc;
}

// Hàm phát hành (thêm) vé mới
int phatHanhVe(VeSuKien ds[], int *soLuong, VeSuKien veMoi) {
    if (*soLuong >= MAX_VE) {
        printf('Loi: Danh sach da dat suc chua toi đa MAX_VE = 50. Khong the phat hành them ve!\n');
        return 0;
    }
    ds[*soLuong] = veMoi;
    (*soLuong)++;
    return 1;
}

// Hàm soát vé tại cổng QR
int soatVeQR(VeSuKien ds[], int soLuong, int maVe) {
    if (soLuong == 0) {
        printf('Loi: Danh sach ve hien tai dang rong (Underflow)!\n');
        return 0;
    }
    int index = timVeTheoMa(ds, soLuong, maVe);
    if (index == -1) {
        printf('Loi: Ma ve %d khong ton tai trong he thong!\n', maVe);
        return 0;
    }
    if (ds[index].daCheckIn == 1) {
        printf('CANH BAO GIAN LAN: Ma ve %d da duoc check-in truoc do! Tu choi truy cap.\n', maVe);
        return 0;
    }
    ds[index].daCheckIn = 1;
    printf('Soat ve thanh cong cho ma ve %d - Khu vuc: %s!\n', maVe, ds[index].loaiVe);
    return 1;
}

// Hàm hủy vé
int huyVe(VeSuKien ds[], int *soLuong, int maVe) {
    if (*soLuong == 0) {
        printf('Loi: Khong the huy vi danh sach ve dang rong!\n');
        return 0;
    }
    int index = timVeTheoMa(ds, *soLuong, maVe);
    if (index == -1) {
        printf('Loi: Khong tim thay ma ve %d de thuc hien huy!\n', maVe);
        return 0;
    }
    // Giải thuật dồn mảng
    for (int i = index; i < *soLuong - 1; i++) {
        ds[i] = ds[i + 1];
    }
    (*soLuong)--;
    printf('Huy ve co ma %d thanh công. Cap nhat so luong ve hien tai: %d\n', maVe, *soLuong);
    return 1;
}

// Hàm hiển thị danh sách vé
void hienThiDanhSach(VeSuKien ds[], int soLuong) {
    printf('\n--- DANH SACH VE SU KIEN LIVE CONCERT VIBE 2025 (Tong so: %d) ---\n', soLuong);
    printf('%-8s | %-20s | %-10s | %-12s | %-12s | %-10s\n', 'Ma Ve', 'Ten Su Kien', 'Loai Ve', 'Gia Goc', 'Gia Thanh Toan', 'Check-In');
    printf('---------------------------------------------------------------------------------------\n');
    for (int i = 0; i < soLuong; i++) {
        float giaTT = tinhGiaThanhToan(ds[i]);
        printf('%-8d | %-20s | %-10s | %-12.0f | %-12.0f | %-10s\n',
               ds[i].maVe,
               ds[i].tenSuKien,
               ds[i].loaiVe,
               ds[i].giaGoc,
               giaTT,
               ds[i].daCheckIn == 1 ? 'Da Check-In' : 'Chua Check-In');
    }
    printf('---------------------------------------------------------------------------------------\n\n');
}

int main() {
    VeSuKien danhSach[MAX_VE];
    int soLuongVe = 0;

    printf('=== KHOI TAO DU LIEU BAN DAU ===\n');
    phatHanhVe(danhSach, &soLuongVe, (VeSuKien){1001, 'Live Concert VIBE', 'VIP', 2000000, 1, 0});
    phatHanhVe(danhSach, &soLuongVe, (VeSuKien){1002, 'Live Concert VIBE', 'GA', 1000000, 0, 0});
    phatHanhVe(danhSach, &soLuongVe, (VeSuKien){1003, 'Live Concert VIBE', 'Zone A', 1500000, 1, 0});

    // 1. In danh sách ban đầu kèm giá thanh toán thực tế
    hienThiDanhSach(danhSach, soLuongVe);

    // 2. Thử nghiệm phát hành thêm 1 vé mới
    printf('=== 2. THU NGHIEM PHAT HANH THEM VE MOI ===\n');
    phatHanhVe(danhSach, &soLuongVe, (VeSuKien){1004, 'Live Concert VIBE', 'VIP', 2200000, 0, 0});
    hienThiDanhSach(danhSach, soLuongVe);

    // 3. Thực hiện quét mã QR check-in thành công 1 vé
    printf('=== 3. THU NGHIEM SOAT VE QR HOP LE (Ma 1001) ===\n');
    soatVeQR(danhSach, soLuongVe, 1001);

    // 4. Thử nghiệm quét lại chính mã vé vừa check-in để kích hoạt lỗi Duplicate QR
    printf('\n=== 4. THU NGHIEM LOI DUPLICATE QR SCAN (Quét lại mã 1001) ===\n');
    soatVeQR(danhSach, soLuongVe, 1001);

    // 5. Thực hiện hủy 1 vé theo mã (Mã 1002)
    printf('\n=== 5. THU NGHIEM HUY VE (Huy ma 1002) ===\n');
    huyVe(danhSach, &soLuongVe, 1002);

    // 6. Thử nghiệm quét mã vé không tồn tại (Mã 9999)
    printf('\n=== 6. THU NGHIEM MA VE KHONG TON TAI (Quét mã 9999) ===\n');
    soatVeQR(danhSach, soLuongVe, 9999);

    // 7. Hiển thị lại toàn bộ danh sách vé sau các thao tác
    printf('\n=== 7. DANH SACH VE SAU KHI THUC HIEN CAC THAO TAC ===\n');
    hienThiDanhSach(danhSach, soLuongVe);

    return 0;
}