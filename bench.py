import pyperf

runner = pyperf.Runner()

runner.timeit(
    name="Decode with heic2rgb",
    setup="""\
import heic2rgb
with open("0.heic", "rb") as f:
    heic_bytes = f.read()
    """,
    stmt="image = heic2rgb.decode(heic_bytes)",
)

runner.timeit(
    name="Decode with pyheif",
    setup="""\
import pyheif
with open("0.heic", "rb") as f:
    heic_bytes = f.read()
    """,
    stmt="image = pyheif.read(heic_bytes)",
)

runner.timeit(
    name="Decode with pillow_heif",
    setup="""\
import io
from PIL import Image
import pillow_heif
pillow_heif.register_heif_opener()
with open("0.heic", "rb") as f:
    heic_bytes = f.read()
    """,
    stmt="""\
with Image.open(io.BytesIO(heic_bytes)) as image:
    tmp = image.getdata()
    """,
)
