# 🎵 MP3 Tag Reader in C

This command-line utility reads and displays **ID3 tag metadata** from MP3 files using low-level binary parsing in C. It supports both **ID3v1** and **ID3v2** tag formats, extracting fields such as Title, Artist, Album, Year, and more directly from the MP3 file structure.

This project showcases file I/O, byte-level parsing, and structured metadata handling, and can be extended into a full-fledged tag editor.

---

## 🧾 Features

- Detects ID3 version (v1, v2.2, v2.3, v2.4)
- Parses:
  - Title
  - Artist
  - Album
  - Year
  - Genre
  - Track number
- Reads both header and frame structures from binary data
- Command-line interface for ease of use
- Modular, extensible design (can be turned into an editor)

---

## 🎯 Learning Objectives

- Understand how multimedia metadata is stored at the byte level
- Practice bit masking and parsing binary headers
- Explore structured data formats in file systems

---

## 🧰 Tools & Technologies

- **Language:** C
- **Concepts:** File I/O, Bitwise Operations, Binary Data Parsing
- **Standard:** ID3v1 and ID3v2 metadata formats

---

## 📁 Project Structure

/mp3-tag-reader
├── main.c # Program entry point
├── id3v1_parser.c / .h # Parser for ID3v1 tags
├── id3v2_parser.c / .h # Parser for ID3v2 tags
├── utils.c / .h # Utility functions
├── test_song.mp3 # Sample MP3 for testing
├── README.md # Project documentation


---

## 🚀 How to Compile & Run

```bash
gcc main.c id3v1_parser.c id3v2_parser.c utils.c -o mp3reader
./mp3reader test_song.mp3
