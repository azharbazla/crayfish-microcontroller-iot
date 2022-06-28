import numpy as np
import skfuzzy as fuzz
import matplotlib.pyplot as plt

# Generate universe variables
x_ph = np.arange(0, 14, 0.1)
x_amonia = np.arange(0, 1.7, 0.1)

# Generate fuzzy membership functions
ph_1 = fuzz.trimf(x_ph, [0, 0, 7.75])
ph_2 = fuzz.trimf(x_ph, [6.5, 8, 9])
ph_3 = fuzz.trimf(x_ph, [7.75, 14, 14])
amonia_1 = fuzz.trimf(x_amonia, [0, 0, 0.1])
amonia_2 = fuzz.trimf(x_amonia, [0, 0.3, 0.6])
amonia_3 = fuzz.trimf(x_amonia, [0.1, 1.6, 1.6])

# Visualize these universes and membership functions
fig, (ax0, ax1) = plt.subplots(nrows=2, figsize=(10, 10))

ax0.plot(x_ph, ph_1, 'b', linewidth=1.5, label='Rendah')
ax0.plot(x_ph, ph_2, 'g', linewidth=1.5, label='Baik')
ax0.plot(x_ph, ph_3, 'r', linewidth=1.5, label='Tinggi')
ax0.set_title('pH')
ax0.legend()

ax1.plot(x_amonia, amonia_1, 'b', linewidth=1.5, label='Baik')
ax1.plot(x_amonia, amonia_2, 'g', linewidth=1.5, label='Sedang')
ax1.plot(x_amonia, amonia_3, 'r', linewidth=1.5, label='Buruk')
ax1.set_title('amonia')
ax1.legend()

# Turn off top/right axes
for ax in (ax0, ax1, ax2):
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.get_xaxis().tick_bottom()
    ax.get_yaxis().tick_left()

plt.tight_layout()
