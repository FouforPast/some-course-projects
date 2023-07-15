from itertools import chain
class State:
    time1 = 0
    time2 = 0
    counts1 = 0;
    def __init__(self, operation = None, G = 0, parent = None, col = 3,row = 3, initdata = [[None,1,2],[3,4,5],[6,7,8]]):
        self.operation = operation
        self.row = row
        self.col = col
        self.G = G
        self.data = initdata
        self.parent = parent
        self.H = 0
    def __str__(self):
        re = "**************** "
        re += str(self.operation)
        re += '\n'
        for i in range(self.row):
            for j in range(self.col):
                re += (str(self.data[i][j]) + ',')
            re += '\n'
        return re
    def __eq__(self, other):
        if(other == None): return False
        return self.data == other.data
    def __hash__(self):
        return hash(tuple(i for item in self.data for i in item))
    def __lt__(self, other):
        return self.G+self.H < other.G+other.H
    #求启发函数
    # def getH(self,sel):
    #     P = 0#求曼哈顿距离
    #     for i in range(self.row):
    #         for j in range(self.col):
    #             try:
    #                 P += (abs(i - (self.data[i][j] - 1) // self.col) +
    #                       abs(j - (self.data[i][j] - 1) % self.col))
    #             except Exception:
    #                 pass
    #     if(sel == 0):
    #         self.H = max(self.col, self.row) * P
    #     elif(sel == 1):
    #         self.H = max(self.col, self.row) * P
    #         self.H += (1 if self.data[self.row - 1][self.col - 1] != None else 0)
    #         self.H += State.getReverseValue(self.data) * 2
    #     elif(sel == 2):
    #         self.H = P
    #     elif(sel == 3):
    #         self.H = 0
    def getChildren(self):
        flag = 0
        for i in range(self.row):
            for j in range(self.col):
                if(self.data[i][j] == None):
                    flag = 1
                    break
            if(flag):break
        childList = []
        if(i > 0 and self.operation != 1):
            data = [[self.data[i][j] for j in range(self.col)] for i in range(self.row)]
            S = State(0, self.G + 1, self, self.col, self.row, data)
            S.data[i][j], S.data[i - 1][j] = S.data[i - 1][j], S.data[i][j]
            childList.append(S)
        if ((i < self.row - 1) and self.operation != 0):
            data = [[self.data[i][j] for j in range(self.col)] for i in range(self.row)]
            S = State(1, self.G + 1, self, self.col, self.row, data)
            S.data[i][j], S.data[i + 1][j] = S.data[i + 1][j], S.data[i][j]
            childList.append(S)
        if(j > 0 and self.operation != 3):
            data = [[self.data[i][j] for j in range(self.col)] for i in range(self.row)]
            S = State(2, self.G + 1, self, self.col, self.row, data)
            S.data[i][j], S.data[i][j - 1] = S.data[i][j - 1], S.data[i][j]
            childList.append(S)
        if((j < self.col - 1) and self.operation != 2):
            data = [[self.data[i][j] for j in range(self.col)] for i in range(self.row)]
            S = State(3, self.G + 1, self, self.col, self.row, data)
            S.data[i][j], S.data[i][j + 1] = S.data[i][j + 1], S.data[i][j]
            childList.append(S)
        return childList
    # 求序列的逆序值
    @staticmethod
    def getReverseValue(inList):
        lst = list(chain(*inList))
        lst.remove(None)
        value = 0
        for i in range(len(lst)):
            for j in range(i + 1, len(lst)):
                if (lst[j] < lst[i]): value += 1
        return value