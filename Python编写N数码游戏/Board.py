from PyQt5.Qt import *
from PyQt5 import QtGui
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PIL import ImageQt, Image

class Board(QWidget):
    def __init__(self,parent = None,Row= 4,Col = 4,mode = 1):
        super(Board, self).__init__(parent)
        self.mode = mode     #mode = 0:数字；mode = 1：图片
        self.goal = self.updateGoal(Row,Col)
        self.data = self.goal
        self.row = Row
        self.col = Col
        self.sideLength = 500 / max(Row,Col)
        self.image = {}
        self.image[0] = Image.open("./picture/One Peace.jpg")
        self.noneX = None
        self.noneY = None
        self.getNone()
        self.myLayout = QGridLayout()
        self.box = []
        self.buildBox()
    def buildBox(self):
        if(self.mode):
            self.setImage()
        for i in range(self.row):
            temp_box = []
            for j in range(self.col):
                label = QLabel()
                if(self.mode == 0):
                    self.resize(600,600)
                    label.setFixedSize(self.sideLength, self.sideLength)
                    label.setAlignment(Qt.AlignCenter)
                    font = QFont()
                    font.setPointSize(30)
                    label.setFont(font)
                    if (self.data[i][j] != None):
                        label.setText(str(self.data[i][j]))
                        label.setStyleSheet(
                            'background-color:rgb(100,216,200);border: 3px solid blue;border-radius:20px')
                    else:
                        label.setStyleSheet('background-color:transparent')
                else:
                    if(self.image['size'][0] >= self.image['size'][1]):
                        self.resize(600,600*self.image['size'][1]/self.image['size'][0])
                        # label.setFixedSize(self.width() / self.col,
                        #                    (self.width() *self.image['size'][1])/ (self.col * self.image['size'][0]))
                    else:
                        self.resize(600 * self.image['size'][0] / self.image['size'][1],600)
                        # label.setFixedSize((self.height() * self.image['size'][0]) / (self.row * self.image['size'][1]),
                        #                    self.height() / self.row)
                    label.setFixedSize(self.width() / self.col,self.height() / self.row)
                    if(self.data[i][j] != None):
                        # self.image[self.data[i][j]].scaled(Qt.with,label.width(),label.height())
                        label.setPixmap(QtGui.QPixmap.fromImage(self.image[self.data[i][j]]))
                        label.setScaledContents(True)
                        label.setStyleSheet('background-color:transparent')
                    else:
                        label.setPixmap(QPixmap(""))
                        label.setStyleSheet('background-color:transparent')
                temp_box.append(label)
                self.myLayout.addWidget(label,i,j)
            self.box.append(temp_box)
        self.setLayout(self.myLayout)
    #重建一个n×m的box
    def rebulidBoard(self, Row, Col):
        self.goal = self.updateGoal(Row, Col)
        self.data = self.goal
        self.row = Row
        self.col = Col
        self.noneX = None
        self.noneY = None
        self.getNone()
        self.sideLength = 450 / max(Row, Col)
        self.frame = 50 / max(Row, Col)
        for i in range(self.myLayout.count()):
            self.myLayout.itemAt(i).widget().deleteLater()
        del self.box
        self.box = []
        if(self.mode):
            self.setImage()
        self.buildBox()
    def updateData(self, newData):
        self.data = newData
        self.getNone()
        try:
            for i in range(self.row):
                for j in range(self.col):
                    if(self.mode == 0):
                        if (newData[i][j] != None):
                            self.box[i][j].setText(str(self.data[i][j]))
                            self.box[i][j].setStyleSheet(
                                'background-color:rgb(100,216,200);border: 3px solid blue;border-radius:20px')
                        else:
                            self.box[i][j].setText("")
                            self.box[i][j].setStyleSheet('background-color:transparent')
                    else:
                        if (newData[i][j] != None):
                            self.box[i][j].setPixmap(QtGui.QPixmap.fromImage(self.image[self.data[i][j]]))
                            self.box[i][j].setScaledContents(True)
                        else:
                            self.box[i][j].setPixmap(QPixmap(""))
        except Exception as e:
            QMessageBox.warning(self, "对话框", "在updateData()中更新数据时发生了{0}的错误".format(e), QMessageBox.Ok)
    #将图片分割
    def setImage(self):
        size = self.image[0].size
        width = int(size[0]//self.col)
        height = int(size[1]//self.row)
        self.image['size'] = size
        for i in range(self.row):
            for j in range(self.col):
                if(i == (self.row - 1) and j == (self.col - 1)):
                    break
                box = (width * j,height * i, width * (j + 1), height * (i + 1))
                region = self.image[0].crop(box)
                self.image[i * self.col + j + 1] =  ImageQt.ImageQt(region)
    #交换两个box的文字或图片
    def switchPos(self,index1,index2):
        if(self.mode == 0):
            text = self.box[index2[0]][index2[1]].text()
            self.box[index1[0]][index1[1]].setText(text)
            self.box[index1[0]][index1[1]].setStyleSheet(
                'background-color:rgb(100,216,200);border: 3px solid blue;border-radius:20px')
            self.box[index2[0]][index2[1]].setText("")
            self.box[index2[0]][index2[1]].setStyleSheet('background-color:transparent')
        else:
            pix = self.box[index2[0]][index2[1]].pixmap()
            self.box[index1[0]][index1[1]].setPixmap(pix)
            self.box[index1[0]][index1[1]].setScaledContents(True)
            self.box[index2[0]][index2[1]].setPixmap(QPixmap(""))
    #改变行列数时更新目标
    def updateGoal(self, row, col):
        list1 = [[i * col + j + 1 for j in range(col)] for i in range(row)]
        list1[row - 1].pop()
        list1[row - 1].append(None)
        return list1
    #获取空格的位置
    def getNone(self):
        for self.noneY in range(self.row):
            for self.noneX in range(self.col):
                # 这里的data必须为原始的data
                if (self.data[self.noneY][self.noneX] == None):
                    return
    #方块的移动，0：上，1：下，2：左，3：右
    def moveBox(self,direction):
        if (direction == 0):
            self.switchPos((self.noneY, self.noneX), (self.noneY - 1, self.noneX))
            self.noneY -= 1
        elif (direction == 1):
            self.switchPos((self.noneY, self.noneX), (self.noneY + 1, self.noneX))
            self.noneY += 1
        elif (direction == 2):
            self.switchPos((self.noneY, self.noneX), (self.noneY, self.noneX - 1))
            self.noneX -= 1
        elif (direction == 3):
            self.switchPos((self.noneY, self.noneX), (self.noneY, self.noneX + 1))
            self.noneX += 1