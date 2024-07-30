newf = "Time,Voltage\n"
counter = 0
with open('FPGA/mtf-neuron/Output Tests/output.txt', 'r') as f:
    for line in f:
        newf += str(counter) + "," + line.strip() + "\n"
        counter += 1
    f.close()
with open('FPGA/mtf-neuron/Output Tests/voltage.csv', 'w') as f:
    f.write(newf)
    f.close()