import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Board extends JLabel{
    protected int[][] chess = new int[15][15];            // 1：黑色，2：白色，0：没有棋子
    protected List<String> history = new ArrayList<>();   // 对局顺序
    protected final int side = 50;                        // 一个格子的边长
    protected final int diameter = 30;                    // 棋子的直径
    protected final int pointsDiameter = 8;               // 小圆点的直径
    protected int xVal = -1, yVal = -1;                   // 当前鼠标在棋盘的落棋点的横坐标和纵坐标
    protected int number = 0;                             // 当前棋子总数量
    protected int[][] order = new int[15][15];            // 记录各个位置的棋子的下棋顺序
    protected int tmpColor = 1;                           // 当前下棋方的棋子颜色
    protected final Color color;                          // 棋盘颜色


    public Board(Color color){
        this.setLayout(null);
        this.color = color;
    }

    public void paint(Graphics g){
        g.setColor(color);
        g.fillRect(0, 0, this.getWidth(), this.getHeight());
        //画棋盘背景
        g.setColor(Color.BLACK);
        g.setFont(MainWindow.font.deriveFont((float)15));
        // 画横线
        for (int i = 0; i < 15; i++){
            g.drawString(String.valueOf(i + 1), side - 35, side * (i + 1) + 6);
            g.drawLine(side, side * (i + 1), side * 15, side * (i + 1));
        }
        // 画纵线
        for (int i = 0; i < 15; i++){
            g.drawString(String.valueOf((char)('A' + i)), side * (i + 1), side - 20);
            g.drawLine(side * (i + 1), side,side * (i + 1), side * 15);
        }
        // 画黑点
        int[][] tmpLocation = new int[][]{{4, 4}, {12, 4}, {4, 12}, {12, 12}, {8, 8}};
        for(int[] l : tmpLocation){
            g.fillOval(side * l[0] - pointsDiameter / 2, side * l[1] - pointsDiameter / 2, pointsDiameter, pointsDiameter);
        }
        // 画棋子
        for (int i = 0; i < 15; i++){
            for (int j = 0; j < 15; j++){
                if(chess[i][j] == 0){
                    continue;
                }
                int x = side + side * j - diameter / 2;
                int y = side + side * i - diameter / 2;
                if(chess[i][j] == 1){
                    g.setColor(Color.BLACK);
                    g.fillOval(x, y, diameter, diameter);
                    g.setColor(Color.WHITE);
                }
                if(chess[i][j] == 2){
                    g.setColor(Color.WHITE);
                    g.fillOval(x, y, diameter, diameter);
                    g.setColor(Color.BLACK);
                }
                if(order[i][j] < 10){
                    g.drawString(String.valueOf(order[i][j]), side + side * j - 3, side + side * i + 3);
                } else if(order[i][j] < 100){
                    g.drawString(String.valueOf(order[i][j]), side + side * j - 8, side + side * i + 3);
                } else{
                    g.drawString(String.valueOf(order[i][j]), side + side * j - 13, side + side * i + 3);
                }
            }
        }
        // 画光标
        if(xVal >= 1 && xVal <= 15 && yVal >= 1&& yVal <= 15){
            g.setColor(Color.RED);
            g.drawLine(xVal * side - diameter / 2, yVal * side - diameter / 2, xVal * side - diameter / 4, yVal * side - diameter / 2);
            g.drawLine(xVal * side - diameter / 2, yVal * side - diameter / 2, xVal * side - diameter / 2, yVal * side - diameter / 4);
            g.drawLine(xVal * side + diameter / 2, yVal * side - diameter / 2, xVal * side + diameter / 4, yVal * side - diameter / 2);
            g.drawLine(xVal * side + diameter / 2, yVal * side - diameter / 2, xVal * side + diameter / 2, yVal * side - diameter / 4);
            g.drawLine(xVal * side - diameter / 2, yVal * side + diameter / 2, xVal * side - diameter / 4, yVal * side + diameter / 2);
            g.drawLine(xVal * side - diameter / 2, yVal * side + diameter / 2, xVal * side - diameter / 2, yVal * side + diameter / 4);
            g.drawLine(xVal * side + diameter / 2, yVal * side + diameter / 2, xVal * side + diameter / 4, yVal * side + diameter / 2);
            g.drawLine(xVal * side + diameter / 2, yVal * side + diameter / 2, xVal * side + diameter / 2, yVal * side + diameter / 4);
        }
    }

    // 重新开始游戏
    public void resetBoard(){
        clear();
        repaint();
    }

    // 重置各个变量但不重画棋盘
    public void clear(){
        history.clear();
        number = 0;
        tmpColor = 1;
        for (int i = 0; i < 15; i++){
            for (int j = 0; j < 15; j++){
                chess[i][j] = 0;
            }
        }
    }
    
    // 悔棋, 返回一共删去了几个棋子
    public int back(int color){
        int rtn = 0;
        while(history.size() > 0) {
            String[] msg = history.get(history.size() - 1).split("&");
            history.remove(history.size() - 1);
            --number;
            chess[Integer.parseInt(msg[2]) - 1][Integer.parseInt(msg[1]) - 1] = 0;
            ++rtn;
            if(color == Integer.parseInt(msg[0])){
                break;
            }
        }
        repaint();
        return rtn;
    }

    // 得到棋盘信息
    public String getBoard(){
        StringBuilder sb = new StringBuilder();
        for(int i = 0; i < 15; i++){
            for(int j = 0; j < 15; j++){
                sb.append(chess[i][j]);
            }
        }
        return sb.toString();
    }

    // 根据getBoard的结果加载棋盘
    public void loadBoard(String msg){
        int idx = 0;
        for(int i = 0; i < 15; ++i){
            for(int j = 0; j < 15; ++j){
                chess[i][j] = Integer.parseInt(String.valueOf(msg.charAt(idx)));
                ++idx;
            }
        }
        repaint();
    }

    // 得到历史信息
    public String getHistory(){
        StringBuilder sb = new StringBuilder();
        for(String s : history){
            sb.append(s).append("*");
        }
        return sb.toString();
    }

    // 根据getHistory的信息加载历史
    public void loadHistory(String msg){
        history = null;
        history = new ArrayList<>(Arrays.asList(msg.split("\\*")));
        number = 0;
        for(String s: history){
            String[] tmp = s.split("&");
            order[Integer.parseInt(tmp[2]) - 1][Integer.parseInt(tmp[1]) - 1] = ++number;
            tmpColor = Integer.parseInt(tmp[0]) == 2 ? 1 : 2;
        }
    }

    // 检查是否有人胜利
    public int check(){
        // 水平方向
        for(int i = 0; i < 15; i++){
            int count = 0;
            int prev = 0;
            for(int j = 0; j < 15; j++){
                if(chess[i][j] != 0){
                    if(prev == chess[i][j]) {
                        count++;
                    }
                    else{
                        count = 1;
                    }
                }
                else{
                    count = 0;
                }
                prev = chess[i][j];
                if(count == 5){
                    return chess[i][j];
                }
            }
        }
        // 垂直方向
        for(int j = 0; j < 15; j++){
            int count = 0;
            int prev = 0;
            for(int i = 0; i < 15; i++){
                if(chess[i][j] != 0){
                    if(prev == chess[i][j]) {
                        count++;
                    }
                    else{
                        count = 1;
                    }
                }
                else{
                    count = 0;
                }
                prev = chess[i][j];
                if(count == 5){
                    return chess[i][j];
                }
            }
        }
        //45°方向
        for(int cnt = 4; cnt < 25; cnt++){
            int count = 0;
            int prev = 0;
            for(int j = cnt > 14? cnt - 14: 0, i = Math.min(cnt, 14); j < 15&&i >= 0; j++, i--){
                if(chess[i][j] != 0){
                    if(prev == chess[i][j]) {
                        count++;
                    }
                    else{
                        count = 1;
                    }
                }
                else{
                    count = 0;
                }
                prev = chess[i][j];
                if(count == 5){
                    return chess[i][j];
                }
            }
        }
        // 135°方向
        for(int cnt = 4; cnt < 25; cnt++){
            int count = 0;
            int prev = 0;
            for(int i = cnt > 14? 0 : 14 - cnt, j = cnt > 14? cnt - 14 : 0; i < 15&&j < 15; i++, j++){
                if(chess[i][j] != 0){
                    if(prev == chess[i][j]) {
                        count++;
                    }
                    else{
                        count = 1;
                    }
                }
                else{
                    count = 0;
                }
                prev = chess[i][j];
                if(count == 5){
                    return chess[i][j];
                }
            }
        }
        return 0;
    }
}