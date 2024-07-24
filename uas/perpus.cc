#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "perpus";  // Pastikan nama database sesuai
unsigned int port = 3306;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Terhubung ke database dengan sukses." << endl;
        } else {
            cerr << "Koneksi gagal: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init gagal" << endl;
    }
    return conn;
}

void tambah_buku(const string& judul_buku, const string& pengarang, const string& penerbit, const string& tahun_terbit) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO buku (judul_buku, pengarang, penerbit, tahun_terbit, status) VALUES ('" + judul_buku + "', '" + pengarang + "', '" + penerbit + "', '" + tahun_terbit + "', 'tersedia')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "INSERT gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil ditambahkan." << endl;
        }
        mysql_close(conn);
    }
}

void lihat_buku() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM buku")) {
            cerr << "SELECT gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Judul Buku: " << row[1] << ", Pengarang: " << row[2] << ", Penerbit: " << row[3] << ", Tahun Terbit: " << row[4] << ", Status: " << row[5] << endl;
        }
        mysql_free_result(res);
        mysql_close(conn);
    }
}

void hapus_buku(int id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream ss;
        ss << "DELETE FROM buku WHERE id = " << id;
        if (mysql_query(conn, ss.str().c_str())) {
            cerr << "DELETE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

void update_buku(int id, const string& judul_buku, const string& pengarang, const string& penerbit, const string& tahun_terbit) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream ss;
        ss << "UPDATE buku SET judul_buku = '" << judul_buku << "', pengarang = '" << pengarang << "', penerbit = '" << penerbit << "', tahun_terbit = '" << tahun_terbit << "' WHERE id = " << id;
        if (mysql_query(conn, ss.str().c_str())) {
            cerr << "UPDATE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil diperbarui." << endl;
        }
        mysql_close(conn);
    }
}

void ambil_buku(int id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream ss;
        ss << "UPDATE buku SET status = 'diambil' WHERE id = " << id << " AND status = 'tersedia'";
        if (mysql_query(conn, ss.str().c_str())) {
            cerr << "UPDATE gagal: " << mysql_error(conn) << endl;
        } else {
            if (mysql_affected_rows(conn) > 0) {
                cout << "Buku berhasil diambil." << endl;
            } else {
                cout << "Buku tidak tersedia atau sudah diambil." << endl;
            }
        }
        mysql_close(conn);
    }
}

void menu_admin() {
    int pilihan;
    do {
        cout << "Menu Admin" << endl;
        cout << "1. Tambah Buku" << endl;
        cout << "2. Lihat Buku" << endl;
        cout << "3. Hapus Buku" << endl;
        cout << "4. Update Buku" << endl;
        cout << "5. Keluar" << endl;
        cout << "Pilih opsi: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1: {
                string judul_buku, pengarang, penerbit, tahun_terbit;
                cout << "Judul Buku: ";
                getline(cin, judul_buku);
                cout << "Pengarang: ";
                getline(cin, pengarang);
                cout << "Penerbit: ";
                getline(cin, penerbit);
                cout << "Tahun Terbit: ";
                getline(cin, tahun_terbit);
                tambah_buku(judul_buku, pengarang, penerbit, tahun_terbit);
                break;
            }
            case 2:
                lihat_buku();
                break;
            case 3: {
                int id;
                cout << "ID Buku yang akan dihapus: ";
                cin >> id;
                hapus_buku(id);
                break;
            }
            case 4: {
                int id;
                string judul_buku, pengarang, penerbit, tahun_terbit;
                cout << "ID Buku yang akan diperbarui: ";
                cin >> id;
                cin.ignore();
                cout << "Judul Buku baru: ";
                getline(cin, judul_buku);
                cout << "Pengarang baru: ";
                getline(cin, pengarang);
                cout << "Penerbit baru: ";
                getline(cin, penerbit);
                cout << "Tahun Terbit baru: ";
                getline(cin, tahun_terbit);
                update_buku(id, judul_buku, pengarang, penerbit, tahun_terbit);
                break;
            }
            case 5:
                cout << "Keluar dari menu admin." << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }
    } while (pilihan != 5);
}

void menu_user() {
    int pilihan;
    do {
        cout << "Menu User" << endl;
        cout << "1. Lihat Buku" << endl;
        cout << "2. Ambil Buku" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilih opsi: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                lihat_buku();
                break;
            case 2: {
                int id;
                cout << "ID Buku yang akan diambil: ";
                cin >> id;
                ambil_buku(id);
                break;
            }
            case 3:
                cout << "Keluar dari menu user." << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }
    } while (pilihan != 3);
}

int main() {
    int role;
    cout << "Masukkan peran (1 untuk Admin, 2 untuk User): ";
    cin >> role;
    cin.ignore();

    if (role == 1) {
        menu_admin();
    } else if (role == 2) {
        menu_user();
    } else {
        cout << "Peran tidak valid." << endl;
    }

    return 0;
}
