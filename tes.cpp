#include <iostream>
#include <cstring>
#include <fstream> 

using namespace std;

// --- 1. Definisi Struktur Data ---
typedef struct {
    char nama[100];
    int pengurangan; 
} Proyek;

typedef struct {
    Proyek data[100]; 
    int size;         
} MaxHeap;

// --- 2. Fungsi Helper & Status Heap ---
void initHeap(MaxHeap *heap) {
    heap->size = 0;
}

bool isEmpty(MaxHeap *heap) {
    return heap->size == 0;
}

bool isFull(MaxHeap *heap) {
    return heap->size == 100;
}

void swapProyek(Proyek *a, Proyek *b) {
    Proyek temp = *a;
    *a = *b;
    *b = temp;
}

// --- 3. Logika Max-Heap (Heapify) ---
void heapifyUp(MaxHeap *heap, int index) {
    if (index == 0) return; 

    int parentIndex = (index - 1) / 2;
    if (heap->data[index].pengurangan > heap->data[parentIndex].pengurangan) {
        swapProyek(&heap->data[index], &heap->data[parentIndex]);
        heapifyUp(heap, parentIndex); 
    }
}

void heapifyDown(MaxHeap *heap, int index) {
    int largest = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    if (leftChild < heap->size && heap->data[leftChild].pengurangan > heap->data[largest].pengurangan) {
        largest = leftChild;
    }

    if (rightChild < heap->size && heap->data[rightChild].pengurangan > heap->data[largest].pengurangan) {
        largest = rightChild;
    }

    if (largest != index) {
        swapProyek(&heap->data[index], &heap->data[largest]);
        heapifyDown(heap, largest); 
    }
}

// --- 4. Fungsi Wajib Program ---
void insert(MaxHeap *heap, const char* nama, int pengurangan) {
    if (isFull(heap)) {
        cout << "[Gagal] Antrian Penuh!" << endl;
        return;
    }

    int currentIndex = heap->size;
    strcpy(heap->data[currentIndex].nama, nama);
    heap->data[currentIndex].pengurangan = pengurangan;
    heap->size++;

    heapifyUp(heap, currentIndex);
}

void extractMax(MaxHeap *heap) {
    if (isEmpty(heap)) {
        cout << "[Info] Antrian Kosong." << endl;
        return;
    }

    Proyek maxProyek = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);

    cout << "\n--------------------------------------------------" << endl;
    cout << "[EXTRACT MAX] Proyek Diambil: " << maxProyek.nama << endl;
    cout << "Potensi Emisi: " << maxProyek.pengurangan << " kg CO2e/tahun" << endl;
    cout << "--------------------------------------------------" << endl;
}

void peekMax(MaxHeap *heap) {
    if (isEmpty(heap)) {
        cout << "[Info] Antrian Kosong." << endl;
        return;
    }
    cout << "\n[PEEK] Prioritas Tertinggi: " << heap->data[0].nama 
         << " (" << heap->data[0].pengurangan << " kg CO2e)" << endl;
}

void displayHeap(MaxHeap *heap) {
    if (isEmpty(heap)) {
        cout << "[Info] Antrian Proyek Kosong." << endl;
        return;
    }

    cout << "\n=== DAFTAR SEMUA PROYEK (URUTAN HEAP) ===" << endl;
    for (int i = 0; i < heap->size; i++) {
        cout << i + 1 << ". " << heap->data[i].nama 
             << " - " << heap->data[i].pengurangan << " kg CO2e" << endl;
    }
    cout << "=========================================" << endl;
}

// --- 5. Fitur Bonus ---
void hitungTotal(MaxHeap *heap) {
    long long total = 0;
    for (int i = 0; i < heap->size; i++) {
        total += heap->data[i].pengurangan;
    }
    cout << "\n[BONUS] Total Potensi Reduksi: " << total << " kg CO2e" << endl;
}

void displayTop5(MaxHeap heapAsli) { 
    cout << "\n[BONUS] Top 5 Proyek Prioritas Tertinggi:" << endl;
    int count = 0;
    while (!isEmpty(&heapAsli) && count < 5) {
        Proyek top = heapAsli.data[0];
        cout << count + 1 << ". " << top.nama << " (" << top.pengurangan << ")" << endl;
        
        heapAsli.data[0] = heapAsli.data[heapAsli.size - 1];
        heapAsli.size--;
        heapifyDown(&heapAsli, 0);
        count++;
    }
}

void saveToFile(MaxHeap *heap) {
    ofstream file("data_proyek.txt");
    if (file.is_open()) {
        file << heap->size << endl; 
        for (int i = 0; i < heap->size; i++) {
            file << heap->data[i].pengurangan << endl;
            file << heap->data[i].nama << endl;
        }
        file.close();
        cout << "[BONUS] Data berhasil disimpan." << endl;
    } else {
        cout << "[Error] Gagal membuat file." << endl;
    }
}

void loadFromFile(MaxHeap *heap) {
    ifstream file("data_proyek.txt");
    if (file.is_open()) {
        int jumlahData;
        file >> jumlahData;
        char namaTemp[100];
        int nilaiTemp;
        
        for (int i = 0; i < jumlahData; i++) {
            file >> nilaiTemp;
            file.ignore(); 
            file.getline(namaTemp, 100);
            insert(heap, namaTemp, nilaiTemp);
        }
        file.close();
        cout << "[BONUS] Data berhasil dimuat." << endl;
    } else {
        cout << "[Error] File tidak ditemukan." << endl;
    }
}

// --- MAIN PROGRAM (PERBAIKAN ALUR) ---

int main() {
    MaxHeap heap;
    initHeap(&heap);

    // 1. MEMUAT 6 PROYEK AWAL
    cout << "Sedang memuat data awal..." << endl;
    insert(&heap, "Pasang panel surya di gedung kuliah", 50000);
    insert(&heap, "Ganti lampu ke LED hemat energi", 20000);
    insert(&heap, "Program tanam pohon di sekitar kampus", 10000);
    insert(&heap, "Kampanye hemat listrik", 5000);
    insert(&heap, "Gunakan sepeda kampus", 15000);
    insert(&heap, "Bangun stasiun pengisian mobil listrik", 30000);
    
    cout << "[SUKSES] 6 Proyek Awal Berhasil Dimuat!" << endl;

    // --- PERBAIKAN: Menampilkan seluruh proyek TEPAT sebelum menu dimuat ---
    displayHeap(&heap); 
    // -----------------------------------------------------------------------

    int pilihan;
    char namaInput[100];
    int nilaiInput;

    // 2. MENU UTAMA (Loop)
    do {
        cout << "\n==============================================" << endl;
        cout << "   SISTEM PRIORITAS PROYEK REDUKSI EMISI      " << endl;
        cout << "==============================================" << endl;
        cout << "1. Tambah Proyek Baru (Insert)" << endl;
        cout << "2. Lihat Prioritas Tertinggi (Peek)" << endl;
        cout << "3. Ambil Proyek Tertinggi (Extract)" << endl;
        cout << "4. Tampilkan Seluruh Proyek" << endl;
        cout << "5. FITUR BONUS" << endl;
        cout << "6. Keluar" << endl;
        cout << "Pilihan > ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                cout << "\n--- Tambah Proyek Baru ---" << endl;
                cout << "Masukkan Potensi Reduksi: ";
                cin >> nilaiInput;
                cin.ignore(); 
                cout << "Masukkan Nama Proyek: ";
                cin.getline(namaInput, 100);
                insert(&heap, namaInput, nilaiInput);
                cout << "[Sukses] Proyek ditambahkan." << endl;
                break;

            case 2:
                peekMax(&heap);
                break;

            case 3:
                extractMax(&heap);
                break;

            case 4:
                displayHeap(&heap);
                break;

            case 5:
                int bonusPilihan;
                cout << "\n--- MENU BONUS ---" << endl;
                cout << "1. Hitung Total Reduksi" << endl;
                cout << "2. Top 5 Proyek" << endl;
                cout << "3. Simpan ke File" << endl;
                cout << "4. Muat dari File" << endl;
                cout << "Pilihan Bonus > ";
                cin >> bonusPilihan;

                if (bonusPilihan == 1) hitungTotal(&heap);
                else if (bonusPilihan == 2) displayTop5(heap); 
                else if (bonusPilihan == 3) saveToFile(&heap);
                else if (bonusPilihan == 4) loadFromFile(&heap);
                break;

            case 6:
                cout << "Program Selesai." << endl;
                break;

            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 6);

    return 0;
}