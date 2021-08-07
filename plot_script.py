import matplotlib.pyplot as plt

def main():
    file = open("saidaBF.txt", 'r')
    tempos_bf = []
    tempos_dc4 = []
    tempos_dc3 = []
    graus = []

    for line in file:
        valores = line.split(',')
        graus.append(valores[0])
        tempos_bf.append(float(valores[1]))
        tempos_dc4.append(float(valores[2]))
        tempos_dc3.append(float(valores[3]))

    file.close()

    plt.plot(graus, tempos_dc3)
    plt.show()

main()