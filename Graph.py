import pandas as pd
import matplotlib.pyplot as plt

patients = [
    {"name": "Abil", "age": 19, "gender": "Pria", "bpjs_number": "999", "address": "Kampung Bulang", "disease": "Gigi", "queue_number": 1},
    {"name": "Rafi", "age": 25, "gender": "Pria", "bpjs_number": "888", "address": "Kota Baru", "disease": "Jantung", "queue_number": 2},
    {"name": "Dinda", "age": 30, "gender": "Wanita", "bpjs_number": "777", "address": "Jakarta", "disease": "Kulit", "queue_number": 3},
    {"name": "Rizky", "age": 22, "gender": "Pria", "bpjs_number": "666", "address": "Bandung", "disease": "Mata", "queue_number": 4},
    {"name": "Fadhil", "age": 45, "gender": "Pria", "bpjs_number": "555", "address": "Surabaya", "disease": "Gizi", "queue_number": 5},
    {"name": "Aisyah", "age": 35, "gender": "Wanita", "bpjs_number": "444", "address": "Yogyakarta", "disease": "Kandungan", "queue_number": 6},
    {"name": "Naufal", "age": 28, "gender": "Pria", "bpjs_number": "333", "address": "Medan", "disease": "Anak", "queue_number": 7},
    {"name": "Citra", "age": 40, "gender": "Wanita", "bpjs_number": "222", "address": "Palembang", "disease": "Bedah", "queue_number": 8},
    {"name": "Yusuf", "age": 50, "gender": "Pria", "bpjs_number": "111", "address": "Makassar", "disease": "Ortopedi", "queue_number": 9},
    {"name": "Nabila", "age": 27, "gender": "Wanita", "bpjs_number": "110", "address": "Semarang", "disease": "Jantung", "queue_number": 10},
    {"name": "Andre", "age": 34, "gender": "Pria", "bpjs_number": "109", "address": "Malang", "disease": "Kulit", "queue_number": 11},
    {"name": "Tania", "age": 24, "gender": "Wanita", "bpjs_number": "108", "address": "Padang", "disease": "Immunologi", "queue_number": 12},
    {"name": "Dewi", "age": 32, "gender": "Wanita", "bpjs_number": "107", "address": "Bali", "disease": "Gizi", "queue_number": 13},
    {"name": "Arif", "age": 29, "gender": "Pria", "bpjs_number": "106", "address": "Pontianak", "disease": "Kandungan", "queue_number": 14},
    {"name": "Farah", "age": 36, "gender": "Wanita", "bpjs_number": "105", "address": "Manado", "disease": "Anak", "queue_number": 15},
    {"name": "Gilang", "age": 38, "gender": "Pria", "bpjs_number": "104", "address": "Palangkaraya", "disease": "Bedah", "queue_number": 16},
    {"name": "Dian", "age": 26, "gender": "Wanita", "bpjs_number": "103", "address": "Ambon", "disease": "Ortopedi", "queue_number": 17},
    {"name": "Kirana", "age": 29, "gender": "Wanita", "bpjs_number": "102", "address": "Kendari", "disease": "Jantung", "queue_number": 18},
    {"name": "Hasan", "age": 41, "gender": "Pria", "bpjs_number": "101", "address": "Palu ", "disease": "Kulit", "queue_number": 19},
    {"name": "Sari", "age": 33, "gender": "Wanita", "bpjs_number": "100", "address": "Banjarmasin", "disease": "Gizi", "queue_number": 20},
    {"name": "Rudi", "age": 37, "gender": "Pria", "bpjs_number": "099", "address": "Bali", "disease": "Mata", "queue_number": 21},
    {"name": "Lina", "age": 31, "gender": "Wanita", "bpjs_number": "098", "address": "Jakarta", "disease": "Anak", "queue_number": 22},
    {"name": "Eko", "age": 26, "gender": "Pria", "bpjs_number": "097", "address": "Surabaya", "disease": "Jantung", "queue_number": 23},
    {"name": "Maya", "age": 30, "gender": "Wanita", "bpjs_number": "096", "address": "Bandung", "disease": "Kulit", "queue_number": 24},
    {"name": "Budi", "age": 34, "gender": "Pria", "bpjs_number": "095", "address": "Medan", "disease": "Gizi", "queue_number": 25},
    {"name": "Siti", "age": 29, "gender": "Wanita", "bpjs_number": "094", "address": "Yogyakarta", "disease": "Anak", "queue_number": 26},
    {"name": "Rizki", "age": 32, "gender": "Pria", "bpjs_number": "093", "address": "Palembang", "disease": "Jantung", "queue_number": 27},
    {"name": "Dewi", "age": 30, "gender": "Wanita", "bpjs_number": "092", "address": "Jakarta", "disease": "Gizi", "queue_number": 28},
    {"name": "Andi", "age": 36, "gender": "Pria", "bpjs_number": "091", "address": "Bali", "disease": "Mata", "queue_number": 29},
    {"name": "Tina", "age": 28, "gender": "Wanita", "bpjs_number": "090", "address": "Yogyakarta", "disease": "Anak", "queue_number": 30},
     {"name": "Lina", "age": 26, "gender": "Wanita", "bpjs_number": "089", "address": "Makassar", "disease": "Mata", "queue_number": 31},
    {"name": "Eko", "age": 40, "gender": "Pria", "bpjs_number": "088", "address": "Surabaya", "disease": "Anak", "queue_number": 32},
    {"name": "Tina", "age": 22, "gender": "Wanita", "bpjs_number": "087", "address": "Bandung", "disease": "Gizi", "queue_number": 33},
    {"name": "Fajar", "age": 39, "gender": "Pria", "bpjs_number": "086", "address": "Yogyakarta", "disease": "Bedah", "queue_number": 34},
    {"name": "Rina", "age": 30, "gender": "Wanita", "bpjs_number": "085", "address": "Palembang", "disease": "Ortopedi", "queue_number": 35},
    {"name": "Siti", "age": 27, "gender": "Wanita", "bpjs_number": "084", "address": "Medan", "disease": "Jantung", "queue_number": 36},
    {"name": "Budi", "age": 32, "gender": "Pria", "bpjs_number": "083", "address": "Jakarta", "disease": "Kulit", "queue_number": 37},
    {"name": "Diana", "age": 29, "gender": "Wanita", "bpjs_number": "082", "address": "Bandung", "disease": "Anak", "queue_number": 38},
    {"name": "Andi", "age": 34, "gender": "Pria", "bpjs_number": "081", "address": "Semarang", "disease": "Gizi", "queue_number": 39},
    {"name": "Rizki", "age": 36, "gender": "Pria", "bpjs_number": "080", "address": "Yogyakarta", "disease": "Mata", "queue_number": 40},
    {"name": "Nina", "age": 31, "gender": "Wanita", "bpjs_number": "079", "address": "Bali", "disease": "Bedah", "queue_number": 41},
    {"name": "Fahmi", "age": 28, "gender": "Pria", "bpjs_number": "078", "address": "Surabaya", "disease": "Anak", "queue_number": 42},
    {"name": "Rina", "age": 35, "gender": "Wanita", "bpjs_number": "077", "address": "Medan", "disease": "Jantung", "queue_number": 43},
    {"name": "Siti", "age": 29, "gender": "Wanita", "bpjs_number": "076", "address": "Jakarta", "disease": "Immunologi", "queue_number": 44},
    {"name": "Budi", "age": 33, "gender": "Pria", "bpjs_number": "075", "address": "Bali", "disease": "Anak", "queue_number": 45},
    {"name": "Dewi", "age": 30, "gender": "Wanita", "bpjs_number": "074", "address": "Surabaya", "disease": "Gizi", "queue_number": 46},
    {"name": "Rizki", "age": 32, "gender": "Pria", "bpjs_number": "073", "address": "Bandung", "disease": "Mata", "queue_number": 47},
    {"name": "Tina", "age": 28, "gender": "Wanita", "bpjs_number": "072", "address": "Yogyakarta", "disease": "Anak", "queue_number": 48},
    {"name": "Andi", "age": 36, "gender": "Pria", "bpjs_number": "071", "address": "Palembang", "disease": "Jantung", "queue_number": 49},
    {"name": "Siti", "age": 29, "gender": "Wanita", "bpjs_number": "070", "address": "Jakarta", "disease": "Gizi", "queue_number": 50},
]

# Membuat DataFrame
df = pd.DataFrame(patients)

# Menghitung jumlah pasien per kategori penyakit
disease_counts = df['disease'].value_counts()

# Menampilkan diagram
disease_counts.plot(kind='bar', color='skyblue')
plt.title('Jumlah Pasien Berdasarkan Kategori Penyakit')
plt.xlabel('Kategori Penyakit')
plt.ylabel('Jumlah Pasien')
plt.xticks(rotation=45)
plt.tight_layout()  # Untuk menghindari label yang terpotong
plt.show()

# Membuat DataFrame
df = pd.DataFrame(patients)

# Menghitung jumlah pasien berdasarkan penyakit dan jenis kelamin
disease_gender_count = df.groupby(['disease', 'gender']).size().unstack(fill_value=0)

# Membuat diagram batang
disease_gender_count.plot(kind='bar', stacked=False)
plt.title('Jumlah Pasien Berdasarkan Penyakit dan Jenis Kelamin')
plt.xlabel('Penyakit')
plt.ylabel('Jumlah Pasien')
plt.xticks(rotation=45)
plt.legend(title='Jenis Kelamin')
plt.tight_layout()
plt.show()


clinics = [
    {
        "name": "Klinik Topangado",
        "doctors": [
            {"specialty": "Dokter Spesialis Gigi", "field": "Gigi"},
            {"specialty": "Dokter Spesialis Kulit", "field": "Kulit"},
            {"specialty": "Dokter Spesialis Mata", "field": "Mata"},
            {"specialty": "Dokter Spesialis Gizi", "field": "Gizi"},
        ],
        "doctor_count": 4,
        "patients_count": 0,
    },
    {
        "name": "Klinik Kasih Ibu",
        "doctors": [
            {"specialty": "Dokter Spesialis Kandungan", "field": "Kandungan"},
            {"specialty": "Dokter Spesialis Anak", "field": "Anak"},
            {"specialty": "Dokter Spesialis Bedah", "field": "Bedah"},
            {"specialty": "Dokter Spesialis Gizi", "field": "Gizi"},
        ],
        "doctor_count": 4,
        "patients_count": 0,
    },
    {
        "name": "Klinik Bona Mitra",
        "doctors": [
            {"specialty": "Dokter Spesialis Ortopedi", "field": "Ortopedi"},
            {"specialty": "Dokter Spesialis Jantung", "field": "Jantung"},
            {"specialty": "Dokter Spesialis Immunologi", "field": "Immunologi"},
            {"specialty": "Dokter Spesialis Kulit", "field": "Kulit"},
            {"specialty": "Dokter Spesialis Mata", "field": "Mata"},
        ],
        "doctor_count": 5,
        "patients_count": 0,
    },
    {
        "name": "Klinik Sehat Selalu",
        "doctors": [
            {"specialty": "Dokter Spesialis Gigi", "field": "Gigi"},
            {"specialty": "Dokter Spesialis Jantung", "field": "Jantung"},
            {"specialty": "Dokter Spesialis Gizi", "field": "Gizi"},
            {"specialty": "Dokter Spesialis Mata", "field": "Mata"},
        ],
        "doctor_count": 4,
        "patients_count": 0,
    },
    {
        "name": "Klinik Bunda Rahma",
        "doctors": [
            {"specialty": "Dokter Spesialis Anak", "field": "Anak"},
            {"specialty": "Dokter Spesialis Gigi", "field": "Gigi"},
            {"specialty": "Dokter Spesialis Gizi", "field": "Gizi"},
            {"specialty": "Dokter Spesialis Mata", "field": "Mata"},
            {"specialty": "Dokter Spesialis Kulit", "field": "Kulit"},
        ],
        "doctor_count": 5,
        "patients_count": 0,
    },
    {
        "name": "Klinik Jayadi",
        "doctors": [
            {"specialty": "Dokter Spesialis Jantung", "field": "Jantung"},
            {"specialty": "Dokter Spesialis Immunologi", "field": "Immunologi"},
            {"specialty": "Dokter Spesialis Bedah", "field": "Bedah"},
        ],
        "doctor_count": 3,
        "patients_count": 0,
    },
    {
        "name": "Klinik Pratama Kita",
        "doctors": [
            {"specialty": "Dokter Spesialis Ortopedi", "field": "Ortopedi"},
            {"specialty": "Dokter Spesialis Immunologi", "field": "Immunologi"},
            {"specialty": "Dokter Spesialis Anak", "field": "Anak"},
            {"specialty": "Dokter Spesialis Bedah", "field": "Bedah"},
        ],
        "doctor_count": 4,
        "patients_count": 0,
    },
]


import matplotlib.pyplot as plt
import networkx as nx
import community as community_louvain

# Define the clinics and their specialties
clinics = [
    {
        "name": "Klinik Topangado",
        "specialties": ["Gigi", "Kulit", "Mata", "Gizi"],
    },
    {
        "name": "Klinik Kasih Ibu",
        "specialties": ["Kandungan", "Anak", "Bedah", "Gizi"],
    },
    {
        "name": "Klinik Bona Mitra",
        "specialties": ["Ortopedi", "Jantung", "Immunologi", "Kulit", "Mata"],
    },
    {
        "name": "Klinik Sehat Selalu",
        "specialties": ["Gigi", "Jantung", "Gizi", "Mata"],
    },
    {
        "name": "Klinik Bunda Rahma",
        "specialties": ["Anak", "Gigi", "Gizi", "Mata", "Kulit"],
    },
    {
        "name": "Klinik Jayadi",
        "specialties": ["Jantung", "Immunologi", "Bedah"],
    },
    {
        "name": "Klinik Pratama Kita",
        "specialties": ["Ortopedi", "Immunologi", "Anak", "Bedah"],
    },
]

# Create an undirected graph
G = nx.Graph()

# Add nodes and edges based on shared specialties
for clinic in clinics:
    G.add_node(clinic["name"])  # Add clinic as a node
    for specialty in clinic["specialties"]:
        # Create edges between clinics that share the same specialty
        for other_clinic in clinics:
            if other_clinic["name"] != clinic["name"] and specialty in other_clinic["specialties"]:
                G.add_edge(clinic["name"], other_clinic["name"])

# 1. Menghitung derajat setiap node
degree_dict = dict(G.degree())
print("Derajat setiap klinik:")
for clinic, degree in degree_dict.items():
    print(f"{clinic}: {degree}")

# 2. Visualisasi grafik
plt.figure(figsize=(10, 6))
pos = nx.spring_layout(G, seed=42)  # Untuk layout yang konsisten
nx.draw(G, pos, with_labels=True, node_size=2000, node_color='lightgreen', font_size=10, font_weight='bold')
plt.title("Undirected Graph of Clinics and Shared Specialties")
plt.show()

# 3. Menyimpan grafik
plt.figure(figsize=(10, 6))
nx.draw(G, pos, with_labels=True, node_size=2000, 
        node_color=[partition[node] for node in G.nodes()], 
        cmap=cmap, font_size=10, font_weight='bold')
plt.title("Undirected Graph of Clinics with Communities")
plt.savefig("graph_clinics_communities.png")