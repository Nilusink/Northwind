import matplotlib.pyplot as plt
import serial
import re

PORT = "COM19"
BAUD = 9600


PATTERN = r"temp=([0-9.]+)°C,\s*hum=([0-9.]+)%,\s*pressure=([0-9.]+)"


def main() -> None:
    # connect to device
    dev = serial.Serial(PORT, BAUD)

    # interactive mode
    plt.ion()

    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True)

    line_temp, = ax1.plot([], [], label="Temp", color="red")
    line_hum, = ax2.plot([], [], label="Humidity", color="blue")
    line_pres, = ax3.plot([], [], label="Pressure", color="green")

    ax1.set_ylabel("°C")
    ax2.set_ylabel("%")
    ax3.set_ylabel("hPa")

    ax3.set_xlabel("Samples")

    ax1.grid()
    ax2.grid()
    ax3.grid()

    x_data = []
    temps_data = []
    hums_data = []
    press_data = []

    # ax.set_xlim(0, 100)
    # ax.set_ylim(0, 50)

    try:
        i = 0
        while True:

            # read ALL available lines (but never block hard)
            for _ in range(10):  # limit per frame
                if dev.in_waiting == 0:
                    break

                now_line = dev.readline().decode("utf-8", errors="ignore").strip()

                match = re.search(PATTERN, now_line)

                if match:
                    temp = float(match.group(1))
                    hum = float(match.group(2))
                    pressure = float(match.group(3))

                    temps_data.append(temp)
                    hums_data.append(hum)
                    press_data.append(pressure)

                    x_data.append(i)
                    i += 1

                else:
                    print(f"dev> {now_line}")

            # update plot
            line_temp.set_data(x_data, temps_data)
            line_hum.set_data(x_data, hums_data)
            line_pres.set_data(x_data, press_data)

            ax1.relim()
            ax1.autoscale_view()
            ax2.relim()
            ax2.autoscale_view()
            ax3.relim()
            ax3.autoscale_view()

            plt.draw()
            plt.pause(0.05)

    finally:
        print("done")
        dev.close()

        plt.ioff()
        plt.show()


if __name__ == "__main__":
    main()
