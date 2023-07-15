from PyQt5.Qt import *
from Board import Board
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from State import State
from itertools import chain
from PIL import Image
import time
import numpy
import random
class Window(QWidget):
    def __init__(self, Row = 4,Col = 4,parent = None):
        super(Window, self).__init__(parent)
        self.setWindowTitle("N数码问题")
        self.setFixedSize(1000,800)
        self.setBackground()
        self.time = 0
        self.time2 = 0
        self.mythread = SearchThread()
        self.mytimer = QTimer()
        self.mytimer.setInterval(100)
        self.mytimer.timeout.connect(self.timer)
        self.demoTimer = QTimer()
        self.demoTimer.timeout.connect(self.demo)
        #self.setStyleSheet("border-image:url(./background.jpg);")
        self.window = QWidget(self)
        self.window.resize(self.size())
        self.window.setStyleSheet("color:white")
        self.board = Board(self.window,Row,Col,1)
        self.board.setGeometry(50,100,600,600)
        #图片模式
        self.imageButton = QPushButton("图片模式", self.window)
        self.imageButton.setGeometry(500, 10, 120, 40)
        self.imageButton.setStyleSheet("background-color: transparent; border: 2px solid white;border-radius: 10px")
        self.imageButton.clicked.connect(lambda: self.setMode(1))
        #数字模式
        self.imageButton = QPushButton("数字模式", self.window)
        self.imageButton.setGeometry(500, 60, 120, 40)
        self.imageButton.setStyleSheet("background-color: transparent; border: 2px solid white;border-radius: 10px")
        self.imageButton.clicked.connect(lambda: self.setMode(0))
        #随机生成按钮
        self.randButton = QPushButton("随机生成", self.window)
        self.randButton.setGeometry(800, 365, 175, 50)
        self.randButton.setStyleSheet("background-color: transparent; border: 2px solid white;border-radius: 10px")
        self.randButton.clicked.connect(lambda: self.randButtonClicked(self.board.row,self.board.col))
        #手动生成按钮
        self.defineButton = QPushButton("手动生成", self.window)
        self.defineButton.setGeometry(800, 300, 175, 50)
        self.defineButton.setStyleSheet("background-color: transparent; border: 2px solid white;border-radius: 10px")
        self.defineButton.clicked.connect(lambda: self.defineButtonClicked())
        #搜索按钮
        self.searchButton = QPushButton("搜索", self.window)
        self.searchButton.setGeometry(800, 495, 175, 50)
        self.searchButton.setStyleSheet("background-color: transparent; border: 2px solid white;border-radius: 10px")
        self.searchButton.clicked.connect(lambda: self.searchButtonClicked())
        #演示按钮
        self.autoButton = QPushButton("演示", self.window)
        self.autoButton.setGeometry(800, 430, 175, 50)
        self.autoButton.setStyleSheet("background-color: transparent; border: 2px solid white;border-radius: 10px")
        self.autoButton.clicked.connect(lambda: self.autoButtonClicked())
        #文本框输入
        self.input = QTextEdit(self.window)
        self.input.setPlaceholderText("在此输入你想要的输入的数据，中间用英文逗号隔开，空白格用0表示")
        self.input.setGeometry(700,150,275,130)
        self.input.setStyleSheet("background-color: transparent; border:2px solid white; border-radius: 10px")
        #选择搜索方式
        self.label5 = QLabel("选择搜索方式", self.window)
        self.label5.setGeometry(650, 100, 150, 30)
        self.label5.setStyleSheet("background:transparent;")
        self.comboxSearch = QComboBox(self.window)
        self.comboxSearch.setGeometry(800, 100, 190, 30)
        self.comboxSearch.addItem("快速A搜索(推荐)")
        self.comboxSearch.addItem("双向A搜索(推荐)")
        self.comboxSearch.addItem("优化A搜索")
        self.comboxSearch.addItem("A*搜索(曼哈顿距离)")
        self.comboxSearch.addItem("双向A*搜索)")
        self.comboxSearch.setStyleSheet("background-color: transparent;border:2px solid white")
        self.comboxSearch.setCurrentIndex(0)
        #选择列数和行数框
        self.label1 = QLabel("设置行数和列数", self.window)
        self.label1.setGeometry(650, 50, 150, 30)
        self.label1.setStyleSheet("background:transparent;")
        self.comboxRow = QComboBox(self.window)
        self.comboxRow.setGeometry(800,50,90,30)
        self.comboxRow.setStyleSheet("background-color: transparent;border: 2px solid white;")
        self.comboxCol = QComboBox(self.window)
        self.comboxCol.setGeometry(900,50,90,30)
        self.comboxCol.setStyleSheet("background-color: transparent;border: 2px solid white;")
        for i in range(6):
            self.comboxRow.addItem(str(i + 1))
            self.comboxCol.addItem(str(i + 1))
        self.comboxRow.setCurrentIndex(3)
        self.comboxCol.setCurrentIndex(3)
        self.comboxRow.currentIndexChanged[int].connect(lambda:self.updateBoard())
        self.comboxCol.currentIndexChanged[int].connect(lambda:self.updateBoard())
        #选择演示速度的滑轨
        self.label2 = QLabel("设置演示速度",self.window)
        self.label2.setGeometry(20,50,150,30)
        self.label2.setStyleSheet("background:transparent;")
        self.slider = QSlider(Qt.Horizontal,self.window)
        self.slider.setMinimum(1)
        self.slider.setMaximum(100)
        self.slider.setSingleStep(5)
        self.slider.setValue(20)
        self.slider.setGeometry(150,50,300,30)
        #设置搜索循环次数
        # self.label5 = QLabel("设置最大搜索次数", self.window)
        # self.label5.setGeometry(650, 10, 150, 30)
        # self.label5.setStyleSheet("background:transparent;")
        # self.spinBox = QSpinBox(self.window)
        # self.spinBox.setGeometry(810,10,150,30)
        # self.spinBox.setRange(0,1000000)
        # self.spinBox.setStyleSheet("color:black;")
        # self.spinBox.setSingleStep(50000)
        # self.spinBox.setValue(500000)
        # self.spinBox.valueChanged[int].connect(lambda: self.changeMaxsearch())
        # self.spinBox.setStyleSheet("background-color: transparent;border:2px solid white")
        #进度条
        self.label3 = QLabel("", self.window)
        self.label3.setGeometry(20, 725, 150, 30)
        self.label3.setStyleSheet("background:transparent;")
        self.progressBar = QProgressBar(self.window)
        self.progressBar.setGeometry(100,720,400,40)
        #显示已搜索次数
        self.label8 = QLabel("已搜索循环：", self.window)
        self.label8.setGeometry(525, 700, 150, 30)
        self.label8.setStyleSheet("background:transparent;")
        self.label9 = QLabel("", self.window)
        self.label9.setGeometry(650, 700, 100, 30)
        self.label9.setStyleSheet("background:transparent;border:1px solid white; border-radius: 4px")
        #显示时间
        self.label4 = QLabel("已耗时", self.window)
        self.label4.setGeometry(525, 745, 50, 30)
        self.label4.setStyleSheet("background:transparent;")
        self.lcd = QLCDNumber(self.window)
        self.lcd.setDecMode()
        self.lcd.setDigitCount(5)
        self.lcd.setSegmentStyle(QLCDNumber.Flat)
        self.lcd.setSmallDecimalPoint(False)
        self.lcd.setGeometry(600,740,150,45)
        #自动过滤无解
        self.checkBox = QCheckBox("自动过滤无解情况",self.window)
        self.checkBox.setGeometry(800,675,200,30)
        self.checkBox.setChecked(True)
        # self.checkBox.stateChanged[int].connect()
        #是否有解
        self.label6 = QLabel("状态：",self.window)
        self.label6.setGeometry(800, 725, 50, 30)
        self.label6.setStyleSheet("background:transparent;")
        self.label7 = QLabel("", self.window)
        self.label7.setGeometry(860, 725, 100, 30)
        self.label7.setStyleSheet("background:transparent;border:1px solid white; border-radius: 4px")
        #中止搜索
        self.searchStopButton = QPushButton("停止搜索", self.window)
        self.searchStopButton.setGeometry(800, 560, 175, 50)
        self.searchStopButton.setStyleSheet("background-color: transparent; border: 2px solid white;border-radius: 10px")
        self.searchStopButton.clicked.connect(lambda: self.mythread.stop())
        self.reset()
    def changeFilter(self, state):
        self.filter = state
    # def changeMaxsearch(self):
    #     self.maxSearch == self.spinBox.value()
    #选择图片还是数字模式
    def setMode(self,mode):
        if (self.searching):
            QMessageBox.about(self, "对话框", "正在搜索")
            return
        if (self.demoing):
            QMessageBox.about(self, "对话框", "正在演示")
            return
        self.reset()
        self.board.mode = mode
        if(mode):
            imgName, imgType = QFileDialog.getOpenFileName(self, "打开图片", ".", "*.jpg;;*.png;;All Files(*)")
            if (not imgName):
                return
            image = Image.open(imgName)
            self.board.image[0] = image #存原始图片
            if (image.size[0] >= image.size[1]):
                self.board.move(50, 100 + 300 * (1 - image.size[1] / image.size[0]))
            else:
                self.board.move(50 + 300 * (1 - image.size[0] / image.size[1]), 100)
        else:
            self.board.move(50,100)
        self.board.rebulidBoard(self.board.row,self.board.col)
    def setBackground(self):
        palette = QPalette()
        #设置背景颜色
        #palette.setColor(self.backgroundRole(),QColor(100,100,100))
        #设置背景图片
        palette.setBrush(self.backgroundRole(),QBrush(QPixmap('./picture/background2.jpeg')))
        self.setPalette(palette)
        self.setAutoFillBackground(True)
    def reset(self):
        self.lcd.display(0.00)
        self.searching = False
        self.mytime = 0
        self.path = []
        self.search = False
        self.pathSize = 0
        self.stopDemo = False
        self.demoing = False
        self.label7.setText("")
        # self.maxSearch = self.spinBox.value()
        self.label9.setText("")
        self.label3.setText("进度条")
    def timer(self):
        self.mytime += 1
        self.lcd.display(self.mytime * 0.1)
    def updateBoard(self):
        if(self.searching):
            QMessageBox.about(self, "对话框", "正在搜索")
            return
        if(self.demoing):
            QMessageBox.about(self, "对话框", "正在演示")
            return
        self.reset()
        self.board.rebulidBoard(self.comboxRow.currentIndex() + 1, self.comboxCol.currentIndex() + 1)
    def demo(self):
        if(self.path != []):
            self.demoTimer.setInterval(self.slider.value() * 10)
            self.board.moveBox(self.path[0])
            self.path.pop(0)
            self.progressBar.setValue(self.pathSize - len(self.path))
        else:
            self.board.data = self.board.goal
            self.label3.setText("演示完毕")
            self.demoing = False
            self.search == False
            self.demoTimer.stop()
            self.mytimer.stop()
            return
    #演示按钮
    def autoButtonClicked(self):
        if(self.searching):
            QMessageBox.about(self, "对话框", "正在搜索")
            return
        try:
            if (self.search == False):
                QMessageBox.about(self, "对话框", "路径不可用，请进行搜索或更新数据")
                return
            if(self.pathSize == 0):
                self.label3.setText("演示完毕")
                self.progressBar.setValue(1)
                return
            self.progressBar.setRange(0, self.pathSize)
            self.mytime = 0
            self.mytimer.start()
            self.demoTimer.setInterval(self.slider.value() * 10)
            self.demoTimer.start()
            self.demoing = True
            self.label3.setText("正在演示")
            self.demo()
        except Exception as e:
            QMessageBox.about(self, "警告", "在autoButtonClicked()中发生了{0}的错误".format(e))
    #随机生成按钮
    def randButtonClicked(self,n,m):
        if(self.searching):
            QMessageBox.about(self, "对话框", "正在搜索")
            return
        if(self.demoing):
            QMessageBox.about(self, "对话框", "正在演示")
            return
        self.reset()
        self.progressBar.setRange(0, 1)
        self.progressBar.setValue(0)
        sum = n * m
        lst = [i + 1 for i in range(n * m - 1)]
        lst.append(None)
        while (sum >= 2):
            randnum = random.randint(0, sum - 2)
            lst[randnum], lst[sum - 1] = lst[sum - 1], lst[randnum]
            sum -= 1
        lst = numpy.array(lst).reshape(n, m).tolist()
        if(self.checkBox.isChecked()):
            if(not self.Solvable(lst,self.board.goal,n,m)):
                self.randButtonClicked(n,m)
            else:
                self.board.updateData(lst)
        else:
            self.board.updateData(lst)
    #手动生成按钮
    def defineButtonClicked(self):
        if(self.searching):
            QMessageBox.about(self, "对话框", "正在搜索")
            return
        if(self.demoing):
            QMessageBox.about(self, "对话框", "正在演示")
            return
        self.reset()
        self.progressBar.setRange(0, 1)
        self.progressBar.setValue(0)
        try:
            tempList = self.input.toPlainText().split(',')
            a = tempList.index('0')
            tempList.pop(a)
            tempList = list(map(int, tempList))
            valid = 1
            for i in tempList:
                if (i < 1 or i >= self.board.col * self.board.row):
                    valid = 0
                    break
            if(not valid):
                raise Exception("输入不符合范围")
            tempList.insert(a, None)
            tempList = numpy.array(tempList).reshape(self.board.row, self.board.col).tolist()
            self.board.updateData(tempList)
        except Exception as e:
            QMessageBox.warning(self, "对话框", "您输入的数据非法", QMessageBox.Ok)
    #搜索线程结束后调用的函数
    def searchEnd(self,path,counts):
        self.time2 = time.time() - self.time2
        self.mytimer.stop()
        if (counts == -1):
            QMessageBox.about(self, "对话框", "搜索已停止，用时{0}s".format(self.time2))
            self.progressBar.setRange(0, 1)
            self.progressBar.setValue(0)
            self.searching = False
            self.label3.setText("进度条")
            return
        self.path = path
        self.pathSize = len(self.path)
        self.search = True
        self.searching = False
        self.progressBar.setRange(0, 1)
        self.progressBar.setValue(1)
        self.label3.setText("搜索完毕")
        QMessageBox.about(self, "对话框", "搜索完毕，一共有{0}步，搜索了{1}次,用时{2}s".format(len(self.path), counts,self.time2))
    #搜索按钮
    def searchButtonClicked(self):
        if(self.demoing):
            QMessageBox.about(self, "对话框", "正在演示")
            return
        if(self.Solvable(self.board.data,self.board.goal,self.board.row,self.board.col)):
            self.label7.setText("有解")
            self.searching = True
            self.progressBar.setRange(0,0)
            self.label3.setText("正在搜索")
            self.mytime = 0
            self.mytimer.start()
            self.mythread = SearchThread(State(None,0,None,self.board.col,self.board.row,self.board.data),
                                    self.board.goal,self.comboxSearch.currentIndex(),self.label9)
            self.mythread._signal.connect(self.searchEnd)
            self.mythread.start()
            self.time2 = time.time()
        else:
            self.label7.setText("无解")
    # 判断是否有解
    def Solvable(self,Start, Goal, Row, Col):
        if (Col % 2):  # 奇数
            return True if (State.getReverseValue(Start) % 2 == State.getReverseValue(Goal) % 2) else False
        else:  # 偶数
            for i in range(Row):
                if (None in Start[i]): s = i
                if (None in Goal[i]): g = i
            return True if ((State.getReverseValue(Start) - State.getReverseValue(Goal)) % 2
                            == (g - s) % 2) else False
#搜索线程
class SearchThread(QThread):
    _signal = pyqtSignal((list,int))
    def __init__(self,start = None,goal = None,sel = None,label = None):
        super(SearchThread, self).__init__()
        self.Start = start
        self.goal = goal
        self.sel = sel   #选择搜索方式
        self.label = label
        self.stopSignal = 0 #中断信号
    def stop(self):
        self.stopSignal = 1
    def run(self):
        self.stopSignal = 0
        if(self.sel != 1 and self.sel != 4):
            Open = {}
            OpenSet = set()
            ClosedSet = set()
            OpenSet.add(self.Start)
            Open[self.Start.G + self.Start.H] = {self.Start}
            counts = 0
            self.label.setText('0')
            while True:
                if(self.stopSignal == 1):
                    self._signal.emit([], -1)
                    break
                counts += 1
                if(counts % 1000 == 0):
                    self.label.setText(str(counts))
                min1 = min(Open)
                bestOpen = Open[min1].pop()
                if (len(Open[min1]) == 0):
                    Open.pop(min1)
                ClosedSet.add(bestOpen)
                OpenSet.remove(bestOpen)
                if (bestOpen.data == self.goal):
                    temp = bestOpen
                    Path = []
                    while temp.parent != None:
                        Path.append(temp.operation)
                        temp = temp.parent
                    Path.reverse()
                    self._signal.emit(Path, counts)
                    break
                else:
                    childList = bestOpen.getChildren()
                    # 删除出现在Open和Closed中的元素
                    for child in childList:
                        if (child not in OpenSet and child not in ClosedSet):
                            OpenSet.add(child)
                            # 获取H值
                            P = 0  # 求曼哈顿距离
                            for i in range(child.row):
                                for j in range(child.col):
                                    try:
                                        P += (abs(i - (child.data[i][j] - 1) // child.col) +
                                              abs(j - (child.data[i][j] - 1) %  child.col))
                                    except Exception:
                                        pass
                            if (self.sel == 0):
                                child.H = max(child.col, child.row) * P
                            elif (self.sel == 2):
                                child.H = max(child.col, child.row) * P
                                child.H += (1 if child.data[child.row - 1][child.col - 1] != None else 0)
                                child.H += State.getReverseValue(child.data) * 2
                            elif (self.sel == 3):
                                child.H = P
                            elif (self.sel == 4):
                                child.H = 0
                            # child.getH(self.sel)
                            if ((child.G + child.H) in Open):
                                Open[child.G + child.H].add(child)
                            else:
                                Open[child.G + child.H] = {child}
            else:
                self._signal.emit([], -1)
        #双向A搜索
        else:
            Goal = State(None, 0, None, self.Start.col, self.Start.row, self.goal)
            P = 0
            for k in range(self.Start.row):
                for j in range(self.Start.col):
                    try:
                        P += (abs(k - (self.Start.data[k][j] - 1) // self.Start.col) + abs(
                            j - (self.Start.data[k][j] - 1) % self.Start.col))
                    except Exception:
                        pass
            self.Start.H = max(self.Start.col, self.Start.row) * P
            Goal.H = self.Start.H
            loca = []
            startData = list(chain(*self.Start.data))
            for i in range(len(startData) - 1):
                loca.append(startData.index(i + 1))
            # Start
            Sopen = {}
            SopenSet = set()
            SclosedSet = set()
            SopenSet.add(self.Start)
            Sopen[self.Start.G + self.Start.H] = {self.Start}
            # Goal
            Gopen = {}
            GopenSet = set()
            GclosedSet = set()
            GopenSet.add(Goal)
            Gopen[Goal.G + Goal.H] = {Goal}
            counts = 0
            signal = 0
            while True:
                if (self.stopSignal == 1):
                    self._signal.emit([], -1)
                    break
                counts += 1
                if (counts % 1000 == 0):
                    self.label.setText(str(counts))
                # 对Start进行操作
                min1 = min(Sopen)
                bestOpen = Sopen[min1].pop()
                if (len(Sopen[min1]) == 0):
                    Sopen.pop(min1)
                SclosedSet.add(bestOpen)
                SopenSet.remove(bestOpen)
                # 获取bestOpen的后代
                childList = bestOpen.getChildren()
                # 删除出现在Open和Closed中的元素
                for child in childList:
                    if (child in GclosedSet or child in GopenSet):
                        signal = 1
                        inter = child
                        SopenSet.add(child)
                        break
                    if (child not in SopenSet and child not in SclosedSet):
                        SopenSet.add(child)
                        # 获取H值
                        P = 0  # 求曼哈顿距离
                        for i in range(child.row):
                            for j in range(child.col):
                                try:
                                    P += (abs(i - (child.data[i][j] - 1) // child.col) +
                                          abs(j - (child.data[i][j] - 1) % child.col))
                                except Exception:
                                    pass
                        if(self.sel == 1):
                            child.H = max(child.col, child.row) * P
                        else:
                            child.H = 0
                        if ((child.G + child.H) in Sopen):
                            Sopen[child.G + child.H].add(child)
                        else:
                            Sopen[child.G + child.H] = {child}
                # 对Goal进行操作
                min1 = min(Gopen)
                bestOpen = Gopen[min1].pop()
                if (len(Gopen[min1]) == 0):
                    Gopen.pop(min1)
                GclosedSet.add(bestOpen)
                GopenSet.remove(bestOpen)
                # 获取bestOpen的后代
                childList = bestOpen.getChildren()
                # 删除出现在Open和Closed中的元素
                for i in childList:
                    if (i in SopenSet or i in SclosedSet):
                        signal = 1
                        inter = i
                        GopenSet.add(i)
                        break
                    if (i not in GopenSet and i not in GclosedSet):
                        GopenSet.add(i)
                        # 获取H值
                        P = 0
                        for k in range(i.row):
                            for j in range(i.col):
                                if (i.data[k][j] != None):
                                    P += (abs(k - loca[i.data[k][j] - 1] // i.col) +
                                          abs(j - loca[i.data[k][j] - 1] % i.col))
                        if(self.sel == 1):
                            i.H = max(i.row, i.col) * P
                        else:
                            i.H = P
                        if ((i.G + i.H) in Gopen):
                            Gopen[i.G + i.H].add(i)
                        else:
                            Gopen[i.G + i.H] = {i}
                if (signal):
                    # print(inter)
                    inG = (GclosedSet | GopenSet) - ((GclosedSet | GopenSet) - {inter})
                    inS = (SclosedSet | SopenSet) - ((SclosedSet | SopenSet) - {inter})
                    temp = inS.pop()
                    Path = []
                    while temp.parent != None:
                        Path.append(temp.operation)
                        temp = temp.parent
                    Path.reverse()
                    inG = inG.pop()
                    temp = inG
                    while temp.parent != None:
                        operation = (lambda x: (1 - x) if (x <= 1) else (5 - x))(temp.operation)
                        Path.append(operation)
                        temp = temp.parent
                    self._signal.emit(Path, counts)
                    break
            else:
                self._signal.emit([], -1)
class demoThread(QThread):
    def __init__(self, path, board):
        super(demoThread, self).__init__()
        self.path = path
        self.board = board
    def run(self):
        try:
            self.board.moveBox(self.path[0])
            self.path.pop(0)
        except Exception as e:
            QMessageBox.about(self, "警告", f"在类demoThread的run方法中发生了{e}的错误")