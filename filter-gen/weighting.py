import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

# sampling frequency
fs = 48000

# A-weighting
# ka = 7.39705e9
# num: ka * s^4
# den: (s+129.4)^2 * (s+676.7) * (s + 4636) * (s + 76655)^2
# coefficients after expansion:
aNumCoeff = [7397050000, 0, 0, 0, 0]
aDenCoeff = [1, 158881.5, 6734684542.32, 3.34333e13, 2.67496e16, 5.3015e18, 3.08666e20]

Afiltz = signal.lti(*signal.bilinear(aNumCoeff, aDenCoeff, fs))
Awz, Ahz = signal.freqz(Afiltz.num, Afiltz.den)

Asos = signal.tf2sos(Afiltz.num, Afiltz.den, pairing='nearest')
Aw, Ah = signal.sosfreqz(Asos)

plt.figure(1)
plt.semilogx(Aw*fs/(2*np.pi), 20*np.log10(np.abs(Ah).clip(1e-15)),
             label=r'$SOS$')
plt.semilogx(Awz*fs/(2*np.pi), 20*np.log10(np.abs(Ahz).clip(1e-15)),
             label=r'$TF$')
plt.legend()
plt.xlabel('Frequency [Hz]')
plt.ylabel('Magnitude [dB]')
plt.title("A weighting")
plt.grid()

# C-weighting
# kc = 5.91797e9
# num: kc * s^2
# den: (s+129.4)^2 * (s + 76655)^2
# coefficients after expansion:
cNumCoeff = [5917970000, 0, 0]
cDenCoeff = [1, 153568.8, 5915682397.36, 1523273037501.6, 9.83897e13]

Cfiltz = signal.lti(*signal.bilinear(cNumCoeff, cDenCoeff, fs))
Cwz, Chz = signal.freqz(Cfiltz.num, Cfiltz.den)
Csos = signal.tf2sos(Cfiltz.num, Cfiltz.den, pairing='nearest')
Cw, Ch = signal.sosfreqz(Csos)

plt.figure(2)
plt.semilogx(Cw*fs/(2*np.pi), 20*np.log10(np.abs(Ch).clip(1e-15)),
             label=r'$SOS$')
plt.semilogx(Cwz*fs/(2*np.pi), 20*np.log10(np.abs(Chz).clip(1e-15)),
             label=r'$TF$')
plt.legend()
plt.xlabel('Frequency [Hz]')
plt.ylabel('Magnitude [dB]')
plt.title("C weighting")
plt.grid()

