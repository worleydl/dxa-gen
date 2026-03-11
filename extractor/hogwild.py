# Generated mostly by GPT on a boring afternoon
import struct
import os

hog_path = "descent.hog"
out_dir = "hmq_dump"

os.makedirs(out_dir, exist_ok=True)

with open(hog_path, "rb") as f:
    # Check header
    header = f.read(3)
    if header != b"DHF":
        raise ValueError("Not a valid HOG file")

    while True:
        name_bytes = f.read(13)
        if not name_bytes or len(name_bytes) < 13:
            break

        filename = name_bytes.replace(b'\x00', b'').decode("ascii", errors="ignore")
        size_data = f.read(4)
        if len(size_data) < 4:
            break

        size = struct.unpack("<I", size_data)[0]
        data = f.read(size)

        if filename.lower().endswith(('hmq', 'hmqq', 'hmp')):
            out_path = os.path.join(out_dir, filename)
            with open(out_path, "wb") as out:
                out.write(data)
            print(f"Extracted: {filename} ({size} bytes)")
        else:
            print(f"Skipped: {filename}")

print("Done.")
