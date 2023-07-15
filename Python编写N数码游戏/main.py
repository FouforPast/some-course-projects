from PyQt5.Qt import *
from MainWindow import Window
import sys

if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = Window()
    win.show()
    sys.exit(app.exec_())

