#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <vector>
#include <iomanip>
using namespace std;

const int MAX_BUKU = 100;
const int MAX_ANGGOTA = 100;
const int MAX_RAK = 100;

struct Buku {
    int id;
    string judul;
    string lokasiRak;
};

struct Anggota {
    int id;
    string nama;
};

struct Peminjaman {
    int idBuku;
    int idAnggota;
};

struct TreeNode {
    Buku data;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* root = nullptr;

Buku daftarBuku[MAX_BUKU];
Anggota daftarAnggota[MAX_ANGGOTA];
Peminjaman daftarPinjam[MAX_BUKU];
int jumlahBuku = 0, jumlahAnggota = 0, jumlahPinjam = 0;

stack<Peminjaman> riwayatPengembalian;
queue<Peminjaman> antreanPeminjam;


map<string, int> rakToIndex;
string indexToRak[MAX_RAK];
int adjacencyMatrix[MAX_RAK][MAX_RAK] = {0};
int jumlahRak = 0;

void tambahRakJikaBaru(string rak) {
    if (rakToIndex.find(rak) == rakToIndex.end()) {
        rakToIndex[rak] = jumlahRak;
        indexToRak[jumlahRak] = rak;
        jumlahRak++;
    }
}

void updateGraphRak(string rak) {
    tambahRakJikaBaru(rak);
    int idx = rakToIndex[rak];
    adjacencyMatrix[idx][idx] = 1; // Dummy: koneksi ke diri sendiri
}

void tambahBuku() {
    Buku b;
    cout << "Masukkan ID Buku: ";
    cin >> b.id;
    cin.ignore();
    cout << "Masukkan Judul Buku: ";
    getline(cin, b.judul);
    cout << "Masukkan Lokasi Rak: ";
    cin >> b.lokasiRak;

    daftarBuku[jumlahBuku++] = b;

    TreeNode* newNode = new TreeNode{b, nullptr, nullptr};

    if (!root) root = newNode;
    else {
        TreeNode* cur = root;
        while (true) {
            if (b.judul < cur->data.judul) {
                if (cur->left) cur = cur->left;
                else {
                    cur->left = newNode;
                    break;
                }
            } else {
                if (cur->right) cur = cur->right;
                else {
                    cur->right = newNode;
                    break;
                }
            }
        }
    }

    updateGraphRak(b.lokasiRak);
    cout << "Buku berhasil ditambahkan!\n";
}

void tambahAnggota() {
    Anggota a;
    cout << "Masukkan ID Anggota: ";
    cin >> a.id;
    cin.ignore();
    cout << "Masukkan Nama Anggota: ";
    getline(cin, a.nama);
    daftarAnggota[jumlahAnggota++] = a;
    cout << "Anggota berhasil ditambahkan!\n";
}

void pinjamBuku() {
    int idBuku, idAnggota;
    cout << "Masukkan ID Buku: ";
    cin >> idBuku;
    cout << "Masukkan ID Anggota: ";
    cin >> idAnggota;

    Peminjaman p{idBuku, idAnggota};
    daftarPinjam[jumlahPinjam++] = p;
    antreanPeminjam.push(p);

    cout << "Peminjaman dicatat.\n";
}

void kembalikanBuku() {
    if (antreanPeminjam.empty()) {
        cout << "Tidak ada buku yang sedang dipinjam.\n";
        return;
    }

    Peminjaman p = antreanPeminjam.front();
    antreanPeminjam.pop();
    riwayatPengembalian.push(p);

    cout << "Buku berhasil dikembalikan!\n";
}

TreeNode* cariBuku(TreeNode* node, const string& judul) {
    if (!node) return nullptr;
    if (node->data.judul == judul) return node;
    if (judul < node->data.judul) return cariBuku(node->left, judul);
    else return cariBuku(node->right, judul);
}

void cariDanTampilkanBuku() {
    string judul;
    cout << "Masukkan judul buku yang dicari: ";
    cin.ignore();
    getline(cin, judul);

    TreeNode* hasil = cariBuku(root, judul);
    if (hasil) {
        cout << "Buku ditemukan:\n";
        cout << "ID: " << hasil->data.id << ", Judul: " << hasil->data.judul
             << ", Lokasi Rak: " << hasil->data.lokasiRak << endl;
    } else {
        cout << "Buku tidak ditemukan.\n";
    }
}

void tampilkanGraphRak() {
    cout << "\nPeta Lokasi Rak (Adjacency Matrix):\n\n";

    const int lebarKolom = 10; 

    cout << setw(lebarKolom) << " ";
    for (int i = 0; i < jumlahRak; ++i) {
        cout << setw(lebarKolom) << indexToRak[i];
    }
    cout << "\n";

    cout << string(lebarKolom * (jumlahRak + 1), '-') << "\n";

    for (int i = 0; i < jumlahRak; ++i) {
        cout << setw(lebarKolom) << indexToRak[i];
        for (int j = 0; j < jumlahRak; ++j) {
            cout << setw(lebarKolom) << adjacencyMatrix[i][j];
        }
        cout << "\n";
    }
}


void tampilkanSemuaBukuBerdasarkanRak() {
    if (jumlahBuku == 0) {
        cout << "Belum ada buku yang tersedia.\n";
        return;
    }

    map<string, vector<Buku>> bukuPerRak;
    for (int i = 0; i < jumlahBuku; ++i) {
        bukuPerRak[daftarBuku[i].lokasiRak].push_back(daftarBuku[i]);
    }

    cout << "Daftar Buku Berdasarkan Rak:\n";
    for (const auto& rak : bukuPerRak) {
        cout << "Rak " << rak.first << ":\n";
        for (const auto& buku : rak.second) {
            cout << "  - ID: " << buku.id << ", Judul: " << buku.judul << endl;
        }
    }
}

void tampilkanMenu() {
    cout << "\n===== SISTEM PERPUSTAKAAN =====\n";
    cout << "1. Tambah Buku\n";
    cout << "2. Tambah Anggota\n";
    cout << "3. Pinjam Buku\n";
    cout << "4. Kembalikan Buku\n";
    cout << "5. Cari Buku\n";
    cout << "6. Tampilkan Peta Rakç (Graph)\n";
    cout << "7. Tampilkan Semua Buku Berdasarkan Rak\n";
    cout << "8. Keluar\n";
    cout << "Pilih menu: ";
}

int main() {
    int pilihan;
    do {
        tampilkanMenu();
        cin >> pilihan;
        switch (pilihan) {
            case 1: tambahBuku(); break;
            case 2: tambahAnggota(); break;
            case 3: pinjamBuku(); break;
            case 4: kembalikanBuku(); break;
            case 5: cariDanTampilkanBuku(); break;
            case 6: tampilkanGraphRak(); break;
            case 7: tampilkanSemuaBukuBerdasarkanRak(); break;
            case 8: cout << "Terima kasih!\n"; break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 8);

    return 0;
}
