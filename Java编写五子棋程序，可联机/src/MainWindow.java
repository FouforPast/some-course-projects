import javax.swing.*;
import javax.swing.Timer;
import javax.swing.filechooser.FileFilter;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import java.text.SimpleDateFormat;
import java.util.regex.Pattern;

public class MainWindow extends JFrame implements MouseListener, MouseMotionListener {
    private int selfPlayer = 1;                   // 联机模式时的己方玩家
    private int black = 1;                        // 黑棋方的玩家代号
    private boolean oppoReady = false;            // 对方是否准备好
    private boolean isReady = false;              // 自己是否准备好
    private final String encoding = "UTF-8";      // 设置保存文件和网络传输的编码方式
    private final String[] colors = {"黑", "白"};  // 棋子颜色
    private State state = State.STOP;             // 当前状态
    private SocketThread t;                       // 套接字线程
    private SocketConnect sc;                     // 发送信息和接收信息
    private boolean isOnline = false;             // 当前是否是在线模式

    private final JLabel labelState;              // 状态标签
    private final MyTimer timer;                  // 计时器
    private final Board board;                    // 棋盘
    private final MyButton btnPause;
    private final JTextArea messageArea;          // 显示对局信息的区域
    private final JRadioButton btnOffline;
    private final JLabel labelPoint1;             // 玩家1得分
    private final JLabel labelPoint2;             // 玩家2得分
    private final boolean endTimeout;             // 超时时是否结束比赛
    protected static Font font;                   // 各种组件的字体

    private enum State{DISABLE, RUNNING, PAUSE, STOP, WAITING}   // 比赛的状态变量

    public MainWindow(){
        super("Gobang Game");
        endTimeout = false;
        JPanel jp = (JPanel) this.getContentPane();
        jp.setLayout(null);
        jp.addMouseListener(this);
        jp.addMouseMotionListener(this);
        // 字体
        String fontUrl = ".\\Fonts\\zh-cn.otf";
        try {
            font = Font.createFont(Font.TRUETYPE_FONT, new File(fontUrl));
        } catch (FontFormatException | IOException e) {
            font = new Font("幼圆", Font.BOLD, 20);
        }

        // 得分
        JPanel panelPoints = new JPanel();
        labelPoint1 = new JLabel("0");
        labelPoint1.setFont(font.deriveFont((float) 13));
        labelPoint2 = new JLabel("0");
        labelPoint2.setFont(font.deriveFont((float) 13));
        panelPoints.setLayout(new GridLayout(2, 2));
        JLabel label1 = new JLabel("玩家1得分:");
        JLabel label2 = new JLabel("玩家2得分:");
        label1.setFont(font.deriveFont((float) 13));
        label2.setFont(font.deriveFont((float) 13));
        panelPoints.add(label1);
        panelPoints.add(labelPoint1);
        panelPoints.add(label2);
        panelPoints.add(labelPoint2);
        panelPoints.setBounds(1250, 20, 150, 50);
        jp.add(panelPoints);

        // 棋盘
        Color color = new Color(173, 118, 28);
        board = new Board(color);
        board.setBounds(0, 0, 16 * board.side, 16 * board.side);
        jp.add(board);

        // 定时器
        timer = new MyTimer(2, 0);
        timer.setBounds(850, 20, 100, 50);
        jp.add(timer);

        // 状态标签
        labelState = new JLabel();
        changeState(State.STOP);
        labelState.setBounds(1090, 20, 150, 50);
        labelState.setFont(font.deriveFont((float) 13));
        jp.add(labelState);

        // 单机和联机模式选择
        ButtonGroup grpMode = new ButtonGroup();
        JRadioButton btnOnline = new JRadioButton("联机模式");
        btnOnline.setFont(font.deriveFont((float) 13));
        btnOffline = new JRadioButton("单机模式");
        btnOffline.setFont(font.deriveFont((float) 13));
        btnOffline.setSelected(true);
        grpMode.add(btnOnline);
        grpMode.add(btnOffline);
        btnOnline.addActionListener(e -> setOnline());
        btnOffline.addActionListener(e -> setOffline());
        JPanel panelMode = new JPanel();
        panelMode.add(btnOnline, BorderLayout.NORTH);
        panelMode.add(btnOffline, BorderLayout.SOUTH);
        panelMode.setBounds(975, 20, 100, 60);
        jp.add(panelMode);

        // 历史信息
        messageArea = new JTextArea();
        messageArea.setLineWrap(true);   // 设置自动换行
        messageArea.setEditable(false);
        messageArea.setText("对局信息\n");
        messageArea.setBackground(Color.lightGray);
        messageArea.setFont(font.deriveFont((float) 13));
        JScrollPane cp = new JScrollPane(messageArea);
        cp.setBounds(850, 100, 500, 375);
        jp.add(cp);

        // 聊天panel
        JPanel chatPanel = new JPanel();
        chatPanel.setLayout(new FlowLayout());
        JTextField chatField = new JTextField(35);
        chatField.setFont(font.deriveFont((float) 12));
        JButton btnSend = new JButton("发送");
        btnSend.setFont(font.deriveFont((float) 15));
        chatPanel.add(chatField);
        chatPanel.add(btnSend);
        chatPanel.setBounds(750, 495, 700, 40);
        jp.add(chatPanel);
        btnSend.addActionListener(e -> {
            if(state == State.WAITING || state == State.DISABLE){
                return;
            }
            if(isOnline){
                String msg = chatField.getText();
                if(msg.equals("")){
                    JOptionPane.showMessageDialog(null, "发送内容不能为空", "提示", JOptionPane.INFORMATION_MESSAGE);
                    return;
                }
                if(msg.contains("$")){
                    JOptionPane.showMessageDialog(null, "请勿输入$字符", "提示", JOptionPane.INFORMATION_MESSAGE);
                    return;
                }
                if(sc.sendMsg("MESSAGE#" + msg)){
                    chatField.setText("");
                    if(state != State.STOP){ // STOP状态下尚未确定玩家序号
                    messageArea.append(String.format("【玩家%d】(%s)说：%s\n", selfPlayer,
                                new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), msg));
                    } else {
                        messageArea.append(String.format("【你】(%s)说：%s\n", new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), msg));
                    }
                }
            } else{
                JOptionPane.showMessageDialog(null, "您尚未联机", "提示", JOptionPane.INFORMATION_MESSAGE);
            }
        });

        // 按钮panel
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new GridLayout(3, 3, 20, 30));
        buttonPanel.setBounds(850, 550, 500, 220);
        jp.add(buttonPanel);

        // 开始按钮
        MyButton btnStart = new MyButton("开始", color);
        btnStart.addActionListener(e -> startGame());
        buttonPanel.add(btnStart);

        // 暂停按钮
        btnPause = new MyButton("暂停", color);
        btnPause.addActionListener(e -> {
            if (e.getActionCommand().equals("暂停")) {
                if(state != State.RUNNING){
                    return;
                }
                if(isOnline && !sc.sendMsg("PAUSE")){
                    return;
                }
                pauseGame();
            } else {
                if(state != State.PAUSE){
                    return;
                }
                if(isOnline && !sc.sendMsg("CONTINUE")){
                    return;
                }
                continueGame();
            }
        });
        buttonPanel.add(btnPause);

        // 结束按钮
        MyButton btnEnd = new MyButton("结束", color);
        btnEnd.addActionListener(e -> {
            if(state == State.STOP || state == State.WAITING || state == State.DISABLE){
                return;
            }
            if(isOnline){
                JOptionPane.showMessageDialog(null, "当前模式为联机模式，您不能独自结束游戏，您可以选择求和或认输", "提示", JOptionPane.INFORMATION_MESSAGE);
                return;
            }
            endGame();
        });
        buttonPanel.add(btnEnd);

        // 重新开始按钮
        MyButton btnRestart = new MyButton("重新开始", color);
        btnRestart.addActionListener(e -> restartGame());
        buttonPanel.add(btnRestart);

        // 悔棋按钮
        MyButton btnBack = new MyButton("悔棋", color);
        btnBack.addActionListener(e -> backGame());
        buttonPanel.add(btnBack);

        // 保存按钮
        MyButton btnSave = new MyButton("保存", color);
        btnSave.addActionListener(e -> saveChess());
        buttonPanel.add(btnSave);

        // 加载按钮
        MyButton btnLoad = new MyButton("加载", color);
        btnLoad.addActionListener(e -> loadChess());
        buttonPanel.add(btnLoad);

        // 求和按钮
        MyButton btnTie = new MyButton("求和", color);
        btnTie.addActionListener(e -> tie());
        buttonPanel.add(btnTie);

        // 认输按钮
        MyButton btnDefeat = new MyButton("认输", color);
        btnDefeat.addActionListener(e -> admitDefeat());
        buttonPanel.add(btnDefeat);

        int height = 835;
        int width = 1400;
        showFrame(this, width, height);
        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                super.windowClosing(e);
                if(sc != null){
                    sc.destroy();
                }
                System.exit(0);
            }
        });
    }

    public void restartGame() {
        if(state != State.RUNNING && state != State.PAUSE){
            return;
        }
        if(isOnline) {
            if(!sc.sendMsg("RESTART")){
                changeState(State.WAITING);
                return;
            }
            isReady = true;
            if (oppoReady) {
                resetGame();
            } else {
                labelState.setText("状态:\n己方已准备");
            }
        }
        else{
            resetGame();
        }
    }

    public static void main(String[] args){
        new MainWindow();
    }

    public static void processException(JFrame frame, Exception ex){
        StackTraceElement[] st = ex.getStackTrace();
        JOptionPane.showMessageDialog(frame,  String.format("[类:%s]调用%s时在第%d行代码处发生异常:%s",
                        st[0].getClassName(), st[0].getMethodName(), st[0].getLineNumber(), ex),"错误", JOptionPane.WARNING_MESSAGE);
    }

    // 求和
    public void tie(){
        if(state != State.RUNNING && state != State.PAUSE){
            return;
        }
        if(isOnline){
            if(!sc.sendMsg("TIE")){
                changeState(State.WAITING);
                return;
            }
            messageArea.append(String.format("【玩家1】(%s)：提出求和\n", new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())));
        } else{
            messageArea.append(String.format("【玩家%d】(%s)：进行求和\n", getPlayer(), new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())));
            endGame();
        }
    }

    // 将玩家player的分数增加points分
    public void raisePoints(int player, int points){
        if(player == 1){
            labelPoint1.setText(String.valueOf(Integer.parseInt(labelPoint1.getText()) + points));
        }
        else{
            labelPoint2.setText(String.valueOf(Integer.parseInt(labelPoint1.getText()) + points));
        }
    }

    // 认输
    public void admitDefeat(){
        if(state != State.RUNNING && state != State.PAUSE){
            return;
        }
        if(isOnline){
            if(!sc.sendMsg("DEFEAT")){
                return;
            }
            raisePoints(getOpponent(), 1);
            endGame();
            messageArea.append(String.format("【玩家1】(%s)：认输，游戏结束\n", new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())));
        } else{
            messageArea.append(String.format("【玩家%d】(%s)：认输，游戏结束\n", getPlayer(), new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())));
            endGame();
            raisePoints(getPlayer() == 1? 2 : 1, 1);
        }
    }

    public void setOnline(){
        if(isOnline){
            JOptionPane.showMessageDialog(null, "请先回到单机模式以关闭当前Socket", "提示", JOptionPane.INFORMATION_MESSAGE);
            return;
        }
        if(state != State.STOP){
            JOptionPane.showMessageDialog(null, "请先结束本场游戏", "提示", JOptionPane.INFORMATION_MESSAGE);
            btnOffline.setSelected(true);
            return;
        }
        changeState(State.DISABLE);
        JFrame frameOnline = new JFrame("联机设置");
        frameOnline.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                super.windowClosing(e);
                btnOffline.setSelected(true);
                changeState(State.STOP);
            }
        });
        JPanel jpOnline = (JPanel) frameOnline.getContentPane();

        JTextField tfHost = new JTextField(12);
        JTextField tfPortLocal = new JTextField(12);
        JTextField tfPortOpponent = new JTextField(12);
        JButton btnOK = new JButton("确认");
        JButton btnCancel = new JButton("取消");

        tfHost.setText("localhost");
        tfPortLocal.setText("4567");
        tfPortOpponent.setText("7654");

        // 信息组件
        JPanel p1 = new JPanel();
        p1.setLayout(new GridLayout(3, 2, 20, 15));
        p1.add(new JLabel("对方服务器:"));
        p1.add(tfHost);
        p1.add(new JLabel("对方服务器端口:"));
        p1.add(tfPortOpponent);
        p1.add(new JLabel("本地服务器端口:"));
        p1.add(tfPortLocal);

        // 按钮组件
        JPanel p2 = new JPanel();
        p2.setLayout(new FlowLayout());
        p2.add(btnOK);
        p2.add(btnCancel);

        btnCancel.addActionListener(e -> {
            btnOffline.setSelected(true);
            changeState(State.STOP);
            frameOnline.setVisible(false);
            frameOnline.dispose();
        });
        btnOK.addActionListener(e -> {
            try {
                sc = new SocketConnect(Integer.parseInt(tfPortLocal.getText()),
                        tfHost.getText(), Integer.parseInt(tfPortOpponent.getText()), 2000, encoding);
                isOnline = true;
                t = new SocketThread();
                t.start();
                changeState(State.STOP);
                frameOnline.setVisible(false);
                frameOnline.dispose();
            } catch (Exception ex) {
                processException(frameOnline, ex);
            }
        });
        jpOnline.add(p1, BorderLayout.NORTH);
        jpOnline.add(p2, BorderLayout.SOUTH);
        changeFont(jpOnline);
        showFrame(frameOnline, 300, 200);
    }

    // 让窗口在屏幕中央显示
    public void showFrame(JFrame frame, int width, int height){
        int screenWidth = (int) Toolkit.getDefaultToolkit().getScreenSize().getWidth();
        int screenHeight = (int) Toolkit.getDefaultToolkit().getScreenSize().getHeight();
        frame.setBounds((screenWidth - width) / 2, (screenHeight - height) / 2, width, height);
        frame.setVisible(true);
        frame.setResizable(false);
    }

    public void setOffline(){
        if(!isOnline){
            return;
        }
        isOnline = false;
        if(state != State.STOP){
            JOptionPane.showMessageDialog(null, "请先结束本场游戏", "提示", JOptionPane.INFORMATION_MESSAGE);
            return;
        }
        sc.destroy();
    }

    // 将游戏恢复初始状态
    public void resetGame(){
        oppoReady = false;
        isReady = false;
        labelState.setText("状态:\n游戏开始");
        messageArea.setText("对局信息\n");
        board.resetBoard();
        timer.reset();
        timer.start();
        changeState(State.RUNNING);
        if(isOnline){
            JOptionPane.showMessageDialog(this, String.format("游戏开始，本场比赛由玩家1执%s棋，玩家2执%s棋，您是玩家%d",
                            colors[getColor(1) - 1], colors[getColor(2) - 1], selfPlayer),
                    "提示", JOptionPane.INFORMATION_MESSAGE);
        } else{
            JOptionPane.showMessageDialog(this, String.format("游戏开始，本场比赛由玩家1执%s棋，玩家2执%s棋",
                            colors[getColor(1) - 1], colors[getColor(2) - 1]),
                    "提示", JOptionPane.INFORMATION_MESSAGE);
        }
    }

    // 开始游戏
    public void startGame(){
        if (state != State.STOP) {
            return;
        }
        if(isOnline) {
            selfPlayer = new Random().nextInt(2) + 1;
            black = new Random().nextInt(2) + 1;
            if(!sc.sendMsg(String.format("READY#%d#%d", getOpponent(), black))){
                return;
            }
            isReady = true;
            if (oppoReady) {
                resetGame();
            } else {
                labelState.setText("状态:\n己方已准备");
            }
        }
        else{
            resetGame();
        }
    }


    // 暂停游戏
    public void pauseGame(){
        changeState(State.PAUSE);
        timer.pause();
        btnPause.setText("继续");
    }

    // 继续游戏
    public void continueGame(){
        changeState(State.RUNNING);
        timer.reStart();
        btnPause.setText("暂停");
    }

    // 结束游戏
    public void endGame(){
        JOptionPane.showMessageDialog(null, "游戏结束", "提示", JOptionPane.INFORMATION_MESSAGE);
        changeState(State.STOP);
        timer.reset();
    }

    // 保存棋局按钮
    public void saveChess(){
//        timer.pause();
        JFileChooser jfc = new JFileChooser();
        jfc.setFileFilter(new FileFilter() {
            @Override
            public boolean accept(File f) {
                return f.getName().endsWith(".che");
            }
            @Override
            public String getDescription() {
                return "Chess Document(*.che)";
            }
        });
        jfc.setSelectedFile(new File("New Chess Document.che"));
        if(jfc.showSaveDialog(null) == JFileChooser.APPROVE_OPTION){
            try{
                File f = jfc.getSelectedFile();
                if(f.exists()){
                    if(JOptionPane.showConfirmDialog(null, f.getName() +
                                    " already exists, do you want to replace it?", "Dialog",
                            JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION){
                        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(f), encoding));
                        bw.write(this.getChess());
                        bw.close();
                    }
                }
                else{
                    BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(f), encoding));
                    bw.write(this.getChess());
                    bw.close();
                }
            } catch (Exception exc){
                processException(this, exc);
            }
        }
    }

    // 加载棋局按钮
    public void loadChess(){
        if(state == State.DISABLE){
            return;
        }
        if(state != State.STOP){
            JOptionPane.showMessageDialog(null, "请先结束本场比赛", "提示", JOptionPane.INFORMATION_MESSAGE);
            return;
        }
        timer.reset();
        JFileChooser jfc = new JFileChooser();
        jfc.setMultiSelectionEnabled(false);
        jfc.setFileSelectionMode(JFileChooser.FILES_ONLY);
        jfc.setFileFilter(new FileFilter() {
            @Override
            public boolean accept(File f) {
                return f.getName().endsWith(".che");
            }
            @Override
            public String getDescription() {
                return "Chess Document(*.che)";
            }
        });
        if(jfc.showOpenDialog(null) == JFileChooser.APPROVE_OPTION){
            try{
                InputStream in = new FileInputStream(jfc.getSelectedFile());
                byte[] bytes = new byte[in.available()];
                in.read(bytes);
                in.close();
                loadChessFile(new String(bytes, encoding));
            } catch (Exception exc){
                processException(this, exc);
            }
        }
    }

    // 根据文件信息加载棋局
    public void loadChessFile(String chess){
        String[] sArr = chess.split("\n\\$\n");
        String[] msgPlayer = sArr[3].split("&");
        int[] step = new int[2];
        final boolean[] isOK = {false};
        for(String s : sArr[1].split("\\*")){
            String[] msg = s.split("&");
            step[Integer.parseInt(msg[0]) - 1]++;
        }
        changeState(State.DISABLE);
        JFrame frameLoad = new JFrame("加载棋局");
        frameLoad.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                super.windowClosing(e);
                changeState(State.STOP);
            }
        });

        JPanel jpLoad = (JPanel) frameLoad.getContentPane();
        JComboBox<String> cb = new JComboBox<>();
        cb.addItem("Player1");
        cb.addItem("Player2");
        cb.setEditable(false);
        JTextField tf = new JTextField(12);
        JLabel label = new JLabel(String.format("最大数值为%d", step[0]));
        JButton btnOK = new JButton("确认");
        JButton btnPreview = new JButton("预览");

        JPanel p1 = new JPanel();
        p1.setLayout(new FlowLayout());
        p1.add(cb);
        p1.add(tf);
        p1.add(label);

        JPanel p2 = new JPanel();
        p2.add(btnOK);
        p2.add(btnPreview);

        ActionListener listener = e -> {
            isOK[0] = false;
            int player = Objects.equals(Objects.requireNonNull(cb.getSelectedItem()).toString(), "Player1") ? 1 : 2;
            int color = Integer.parseInt(msgPlayer[0]) == 1? player == 1? 1: 2 : player == 1? 2 : 1; // player所持的颜色
            if(Pattern.compile("^[0-9]+$").matcher(tf.getText()).matches()){
                int maxStep = Integer.parseInt(tf.getText());
                if(maxStep > step[player - 1]) {
                    JOptionPane.showMessageDialog(frameLoad, "您输入的数字过大", "提示", JOptionPane.INFORMATION_MESSAGE);
                } else{
                    // 找到玩家player在maxStep后的棋局
                    int tmpStep = 0;
                    board.clear();
                    for(String s : sArr[1].split("\\*")){
                        if(tmpStep >= maxStep){
                            break;
                        }
                        String[] msg = s.split("&");
                        if(Integer.parseInt(msg[0]) == color){
                            ++tmpStep;
                        }
                        board.chess[Integer.parseInt(msg[2]) - 1][Integer.parseInt(msg[1]) - 1] = Integer.parseInt(msg[0]);
                        board.history.add(s);
                        board.order[Integer.parseInt(msg[2]) - 1][Integer.parseInt(msg[1]) - 1] = ++board.number;
                        board.tmpColor = Integer.parseInt(msg[0]) == 2 ? 1 : 2;
                    }
                    board.repaint();
                    isOK[0] = true;
                }
            } else{
                JOptionPane.showMessageDialog(frameLoad, "您输入的不是自然数", "提示", JOptionPane.INFORMATION_MESSAGE);
            }
        };

        cb.addItemListener(e -> {
            if(e.getStateChange() == ItemEvent.SELECTED){
                int player = Objects.equals(e.getItem().toString(), "Player1") ? 1 : 2;
                label.setText(String.valueOf(String.format("最大数值为%d", step[player - 1])));
            }
        });
        // 按下了确认按钮
        btnOK.addActionListener(e -> {
            // 先执行一遍预览按钮的功能
            listener.actionPerformed(e);
            if(!isOK[0]){
                return;
            }
            if(isOnline) {
                if(!sc.sendMsg(String.format("LOAD#%s#%s#%s#%s", board.getBoard(), board.getHistory(), sArr[2], sArr[3]))){
                    return;
                }
            }
            black = Integer.parseInt(msgPlayer[0]);
            selfPlayer = Integer.parseInt(msgPlayer[2]);
            JOptionPane.showMessageDialog(frameLoad, String.format("游戏开始，本场比赛由玩家1执%s棋，玩家2执%s棋，当前轮到玩家%d下棋",
                            colors[getColor(1) - 1], colors[getColor(2) - 1], getPlayer()),
                    "提示", JOptionPane.INFORMATION_MESSAGE);
            messageArea.setText(sArr[2]);
            frameLoad.setVisible(false);
            frameLoad.dispose();
            timer.start();
            changeState(State.RUNNING);
            labelState.setText("状态:\n游戏开始");
        });
        btnPreview.addActionListener(listener);
        jpLoad.add(p1, BorderLayout.NORTH);
        jpLoad.add(p2, BorderLayout.SOUTH);
        changeFont(jpLoad);
        showFrame(frameLoad, 400, 200);
    }

    // 改变jp里面所有控件的字体
    public void changeFont(JPanel jp){
        for(Component c: jp.getComponents()){
            if(c instanceof JPanel){
                changeFont((JPanel) c);
            } else{
                c.setFont(font.deriveFont((float) c.getFont().getSize()));
            }
        }
    }

    // 获取当前棋局信息
    public String getChess(){
        return board.getBoard() +
                "\n$\n" +
                board.getHistory() +
                "\n$\n" +
                messageArea.getText() +
                "\n$\n" +
                black + "&" + board.tmpColor + "&" + selfPlayer;
    }
    // 得到当前下棋的玩家的序号
    public int getPlayer() {
        return board.tmpColor == 1 ? black : black == 1 ? 2 : 1;
    }
    // 得到对方玩家的代号
    public int getOpponent(){
        return selfPlayer == 2 ? 1 : 2;
    }

    // 得到代号为ID的玩家的棋子颜色
    public int getColor(int ID){
        if(ID == 1){
            return black == 1? 1 : 2;
        }
        else{
            return black == 1? 2 : 1;
        }
    }

    // 悔棋
    public void backGame(){
        if((state != State.RUNNING && state != State.PAUSE) || board.history.size() == 0){
            return;
        }
        if(isOnline){
            if(!sc.sendMsg("BACK")){
                changeState(State.WAITING);
                return;
            }
            messageArea.append("【玩家1】(" + new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()) + ")：提出悔棋\n");
        } else{
            messageArea.append(String.format("【玩家%d】(%s)：悔棋\n", getPlayer(), new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())));
            if(board.back(getColor(getPlayer())) == 1){
                board.tmpColor = board.tmpColor == 2? 1 : 2;
            }
        }
    }

    // 更新鼠标坐标
    public void updateMouseLocation(MouseEvent e){
        board.xVal = ((double) e.getX() / board.side) % 1 > 0.5? e.getX() / board.side + 1:e.getX() / board.side;
        board.yVal = ((double) e.getY() / board.side) % 1 > 0.5? e.getY() / board.side + 1:e.getY() / board.side;
    }

    // 鼠标点击事件
    @Override
    public void mouseClicked(MouseEvent e) {
        if(state != State.RUNNING){
            return;
        }
        if(isOnline && getPlayer() != selfPlayer) {
            return;
        }
        updateMouseLocation(e);
        // 如果鼠标点击在了棋盘上
        if(board.xVal >= 1 && board.xVal <= 15 && board.yVal >= 1&& board.yVal <= 15 && board.chess[board.yVal - 1][board.xVal - 1] == 0){
            if(isOnline && !sc.sendMsg(String.format("LOCATION#%d#%d", board.xVal, board.yVal))){
                return;
            }
            processMouseOnBoard(board.xVal, board.yVal);
        }
    }

    // 处理鼠标点击在棋盘上的事件
    public void processMouseOnBoard(int xVal, int yVal){
        if(timer.isRunning()){
            timer.pause();
        }
        board.chess[yVal - 1][xVal - 1] = board.tmpColor;
        board.order[yVal - 1][xVal - 1] = ++board.number;
        board.history.add(board.tmpColor + "&" + xVal + "&" + yVal);
        board.repaint();
        messageArea.append("【玩家" + getPlayer() + "】" +
                "(" + new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()) + ")：将"
                + (board.tmpColor == 1?"黑棋":"白棋") + "下在了(" + (char) ('A' + xVal - 1)
                + yVal + ")位置\n");

        if(board.check() != 0){
            JOptionPane.showMessageDialog(null, "玩家" + getPlayer() + "胜出",
                    "提示", JOptionPane.INFORMATION_MESSAGE);
            raisePoints(getPlayer(), 1);
            changeState(State.STOP);
            timer.reset();
            return;
        }
        board.tmpColor = board.tmpColor == 2? 1: 2;
        timer.reset();
        timer.reStart();
    }

    private class SocketThread extends Thread{
        public void run(){
            label:
            while(isOnline){
                String msg = sc.getMsg();
                if(msg != null) {
                    String[] msgArray = msg.split("#");
                    switch (msgArray[0]) {
                        case "READY":
                            oppoReady = true;
                            if (isReady) {
                                selfPlayer = Integer.parseInt(msgArray[1]);
                                black = Integer.parseInt(msgArray[2]);
                                resetGame();
                            } else {
                                labelState.setText("状态:\n对方已准备");
                            }
                            break;
                        case "PAUSE":
                            pauseGame();
                            break;
                        case "CONTINUE":
                            continueGame();
                            break;
                        case "LOCATION":
                            processMouseOnBoard(Integer.parseInt(msgArray[1]), Integer.parseInt(msgArray[2]));
                            break;
                        case "TIMEOUT":
                            raisePoints(selfPlayer, 1);
                            timer.reset();
                            JOptionPane.showMessageDialog(null, "对方已超时，游戏结束", "提示", JOptionPane.INFORMATION_MESSAGE);
                            break;
                        case "BACK":
                            messageArea.append(String.format("【玩家%d】(%s)：提出悔棋\n", getOpponent(), new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())));
                            if (JOptionPane.showConfirmDialog(null, "对方提出悔棋，您是否同意？", "Dialog",
                                    JOptionPane.YES_NO_OPTION) == JOptionPane.NO_OPTION) {
                                if (!sc.sendMsg("REFUSE_BACK")) {
                                    continue;
                                }
                                messageArea.append(String.format("【玩家%d】(%s)：拒绝了玩家%d提出的悔棋\n", selfPlayer,
                                        new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), getOpponent()));
                            } else {
                                if (!sc.sendMsg("AGREE_BACK")) {
                                    continue;
                                }
                                if(board.back(getColor(getOpponent())) == 1){
                                    board.tmpColor = board.tmpColor == 2? 1 : 2;
                                }
                                changeState(MainWindow.State.RUNNING);
                                messageArea.append(String.format("【玩家%d】(%s)：同意了玩家%d提出的悔棋\n", selfPlayer,
                                        new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), getOpponent()));
                            }
                            break;
                        case "AGREE_BACK":
                            changeState(MainWindow.State.RUNNING);
                            if(board.back(getColor(selfPlayer)) == 1){
                                board.tmpColor = board.tmpColor == 2? 1 : 2;
                            }
                            messageArea.append(String.format("【玩家%d】(%s)：同意了玩家%d提出的悔棋\n", getOpponent(),
                                    new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), selfPlayer));
                            break;
                        case "REFUSE_BACK":
                            changeState(MainWindow.State.RUNNING);
                            messageArea.append(String.format("【玩家%d】(%s)：拒绝了玩家%d提出的悔棋\n", getOpponent(),
                                    new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), selfPlayer));
                            break;
                        case "CLOSE":  // 关闭socket
                            isOnline = false;
                            sc = null;
                            t = null;
                            btnOffline.setSelected(true);
                            JOptionPane.showMessageDialog(null, "连接已关闭", "提示", JOptionPane.INFORMATION_MESSAGE);
                            break label;
                        case "TIE":
                            messageArea.append(String.format("【玩家%d】(%s)：提出求和\n", getOpponent(), new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())));
                            if (JOptionPane.showConfirmDialog(null, "对方提出求和，您是否同意？", "Dialog",
                                    JOptionPane.YES_NO_OPTION) == JOptionPane.NO_OPTION) {
                                if (!sc.sendMsg("REFUSE_TIE")) {
                                    continue;
                                }
                                messageArea.append(String.format("【玩家%d】(%s)：拒绝了玩家%d提出的求和\n", selfPlayer,
                                        new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), getOpponent()));
                            } else {
                                if (!sc.sendMsg("AGREE_TIE")) {
                                    continue;
                                }
                                messageArea.append(String.format("【玩家%d】(%s)：同意了玩家%d提出的求和\n", selfPlayer,
                                        new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), getOpponent()));
                                endGame();
                            }
                            break;
                        case "REFUSE_TIE":
                            changeState(MainWindow.State.RUNNING);
                            JOptionPane.showMessageDialog(null, "对方拒绝了您的求和请求", "提示", JOptionPane.INFORMATION_MESSAGE);
                            messageArea.append(String.format("【玩家%d】(%s)：拒绝了玩家%d提出的求和\n", getOpponent(),
                                    new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), selfPlayer));
                            break;
                        case "AGREE_TIE":
                            changeState(MainWindow.State.RUNNING);
                            JOptionPane.showMessageDialog(null, "对方同意了您的求和请求", "提示", JOptionPane.INFORMATION_MESSAGE);
                            endGame();
                            messageArea.append(String.format("【玩家%d】(%s)：同意了玩家%d提出的求和\n", getOpponent(),
                                    new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), selfPlayer));
                            break;
                        case "DEFEAT":
                            messageArea.append(String.format("【玩家%d】(%s)：认输，游戏结束\n", getOpponent(), new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())));
                            endGame();
                            raisePoints(selfPlayer, 1);
                            break;
                        case "LOAD":
                            // 加载棋盘
                            board.loadBoard(msgArray[1]);
                            board.loadHistory(msgArray[2]);
                            messageArea.setText(msgArray[3]);
                            String[] msgPlayer = msgArray[4].split("&");
                            black = Integer.parseInt(msgPlayer[0]);
                            selfPlayer = Integer.parseInt(msgPlayer[2]) == 2? 1 : 2;
                            timer.start();
                            changeState(MainWindow.State.RUNNING);
                            labelState.setText("状态:\n游戏开始");
                            JOptionPane.showMessageDialog(null, String.format("游戏开始，本场比赛由玩家1执%s棋，玩家2执%s棋，当前轮到玩家%d下棋",
                                            colors[getColor(1) - 1], colors[getColor(2) - 1], getPlayer()),
                                    "提示", JOptionPane.INFORMATION_MESSAGE);
                            break;
                        case "RESTART":
                            messageArea.append(String.format("【玩家%d】(%s)：提出重新开始\n", getOpponent(), new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())));
                            if (JOptionPane.showConfirmDialog(null, "对方提出重新开始，您是否同意？", "Dialog",
                                    JOptionPane.YES_NO_OPTION) == JOptionPane.NO_OPTION) {
                                if (!sc.sendMsg("REFUSE_RESTART")) {
                                    continue;
                                }
                                changeState(MainWindow.State.RUNNING);
                                messageArea.append(String.format("【玩家%d】(%s)：拒绝了玩家%d提出的重新开始请求\n", selfPlayer,
                                        new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), getOpponent()));
                            } else {
                                selfPlayer = new Random().nextInt(2) + 1;
                                black = new Random().nextInt(2) + 1;
                                if(!sc.sendMsg(String.format("AGREE_RESTART#%d#%d", getOpponent(), black))){
                                    return;
                                }
                                resetGame();
                                messageArea.append(String.format("【玩家%d】(%s)：同意了玩家%d提出的重新开始请求\n", selfPlayer,
                                        new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), getOpponent()));
                            }
                            break;
                        case "AGREE_RESTART":
                            selfPlayer = Integer.parseInt(msgArray[1]);
                            black = Integer.parseInt(msgArray[2]);
                            resetGame();
                            messageArea.append(String.format("【玩家%d】(%s)：同意了玩家%d提出的重新开始请求\n", getOpponent(),
                                    new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), selfPlayer));
                            break;
                        case "REFUSE_RESTART":
                            changeState(MainWindow.State.RUNNING);
                            messageArea.append(String.format("【玩家%d】(%s)：拒绝了玩家%d提出的重新开始请求\n", getOpponent(),
                                    new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), selfPlayer));
                            break;
                        case "MESSAGE":
                            if(state != MainWindow.State.STOP){ // STOP状态下尚未确定玩家序号
                                messageArea.append(String.format("【玩家%d】(%s)说：%s\n", selfPlayer,
                                        new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), msgArray[1]));
                            } else {
                                messageArea.append(String.format("【对方】(%s)说：%s\n", new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()), msgArray[1]));
                            }
                            break;
                    }
                }
            }
        }
    }

    private static class MyButton extends JButton{
        public MyButton(String s, Color color){
            super(s);
            this.setFocusPainted(false);
            this.setBorder(BorderFactory.createEtchedBorder());
            this.setFont(MainWindow.font.deriveFont((float)20));
            this.setBackground(color);
        }
    }

    private class MyTimer extends JLabel implements ActionListener{
        private final Timer _timer;
        private int minutes, seconds;
        private final int oMinutes;
        private final int oSeconds;

        public MyTimer(int minutes, int seconds){
            this.oMinutes = minutes;
            this.oSeconds = seconds;
            _timer = new Timer(1000,  this);
            this.setOpaque(true);
            this.setBackground(Color.darkGray);
            this.setForeground(Color.GREEN);
            this.setFont(new Font("Dialog", Font.PLAIN, 35));
            this.setHorizontalAlignment(JLabel.HORIZONTAL);
            this.reset();
        }

        public void actionPerformed(ActionEvent e) {
            seconds -= 1;
            if(seconds == -1){
                minutes -= 1;
                seconds = 59;
            }
            this.setText((minutes < 10? "0" : "") + minutes + ":" + (seconds < 10? "0" : "") + seconds);
            if(seconds == 0&&minutes == 0){ // 超时
                if(endTimeout) {
                    changeState(State.STOP);
                    raisePoints(getPlayer() == 2 ? 1 : 2, 1);
                    this.reset();
                    if (isOnline) {
                        JOptionPane.showMessageDialog(null, "您已超时，游戏结束", "提示", JOptionPane.INFORMATION_MESSAGE);
                        raisePoints(getOpponent(), 1);
                        sc.sendMsg("TIMEOUT");
                    } else {
                        JOptionPane.showMessageDialog(null, "玩家" + getPlayer() + "超时,游戏结束",
                                "提示", JOptionPane.INFORMATION_MESSAGE);
                    }
                } else{
                    if(isOnline && getPlayer() != selfPlayer){
                        return;
                    }
                    int next = new Random().nextInt(225 - board.number) + 1;
                    int x = 0, y;
                    for(y = 0; y < 15; ++y){
                        for(x = 0; x < 15; ++x){
                            if(board.chess[y][x] == 0){
                                --next;
                            }
                            if(next == 0){
                                break;
                            }
                        }
                        if(next == 0){
                            break;
                        }
                    }
                    if(isOnline && !sc.sendMsg(String.format("LOCATION#%d#%d", x + 1, y + 1))){
                        return;
                    }
                    processMouseOnBoard(x + 1, y + 1);
                }
            }
        }

        public void reset(){
            if(_timer.isRunning()){
                _timer.stop();
            }
            minutes = oMinutes;
            seconds = oSeconds;
            this.setText((minutes < 10? "0" : "") + minutes + ":" + (seconds < 10? "0" : "") + seconds);
        }

        public void start(){
            _timer.start();
        }

        public void pause(){
            if(_timer.isRunning()) {
                _timer.stop();
            }
        }

        public void reStart(){
            if(!_timer.isRunning()){
                _timer.restart();
            }
        }

        public boolean isRunning(){
            return _timer.isRunning();
        }
    }

    public void changeState(State s) {
        state = s;
        if(state == State.RUNNING){
            labelState.setText("状态：\n游戏进行中");
        } else if(state == State.STOP){
            labelState.setText("状态：\n游戏未开始");
        } else if(state == State.PAUSE){
            labelState.setText("状态：\n游戏暂停");
        } else if(state == State.WAITING) {
            labelState.setText("状态：\n等待对方回应");
        } else {
            labelState.setText("状态：\n等待二级页面关闭");
        }
    }

    @Override
    public void mousePressed(MouseEvent e) {}

    @Override
    public void mouseReleased(MouseEvent e) {}

    @Override
    public void mouseEntered(MouseEvent e) {}

    @Override
    public void mouseExited(MouseEvent e) {}

    @Override
    public void mouseDragged(MouseEvent e) {}

    @Override
    public void mouseMoved(MouseEvent e) {
        updateMouseLocation(e);
        board.repaint();
    }
}
