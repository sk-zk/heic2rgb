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
import pillow_heif
import numpy as np
heif_file = pillow_heif.open_heif("0.heic")
    """,
    stmt="""\
np_array = np.asarray(heif_file)
    """,
)
