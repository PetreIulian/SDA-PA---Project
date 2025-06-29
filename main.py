import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file
file_path = "rezultate_cutremure.csv"  # Make sure the CSV file is in the same directory
df = pd.read_csv(file_path)

# Set up the figure and axes
fig, axs = plt.subplots(2, 1, figsize=(12, 10), sharex=True)

# Plot Cutremure vs. MA_Cutremure
axs[0].plot(df['Luna'], df['Cutremure'], marker='o', label='Cutremure')
axs[0].plot(df['Luna'], df['MA_Cutremure'], marker='s', linestyle='--', label='MA_Cutremure')
axs[0].set_title('Număr de Cutremure vs. Media Mobilă')
axs[0].set_ylabel('Număr de Cutremure')
axs[0].legend()
axs[0].grid(True)

# Plot Magnitudine vs. MA_Magnitudine
axs[1].plot(df['Luna'], df['Magnitudine'], marker='o', color='green', label='Magnitudine')
axs[1].plot(df['Luna'], df['MA_Magnitudine'], marker='s', linestyle='--', color='red', label='MA_Magnitudine')
axs[1].set_title('Magnitudine Medie vs. Media Mobilă')
axs[1].set_xlabel('Luna')
axs[1].set_ylabel('Magnitudine')
axs[1].legend()
axs[1].grid(True)

# Display the plots
plt.tight_layout()
plt.show()
