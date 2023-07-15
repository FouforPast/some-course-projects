import javax.swing.*;
import java.io.*;
import java.net.*;

public class SocketConnect{
    private final ServerSocket server;     // 本地服务器套接字
    private final int portOpponent;        // 对方服务器端口
    private final String host;             // 对方服务器地址
    private final int timeout;             // 超时时间
    private final int portLocal;           // 本地服务器端口
    private final String encoding;         // 编码方式

    SocketConnect(int _portLocal, String _host, int _portOpponent, int _timeout, String _encoding) throws Exception{
        portOpponent = _portOpponent;
        host = _host;
        server = new ServerSocket(_portLocal);
        portLocal = _portLocal;
        timeout = _timeout;
        encoding = _encoding;
    }

    public boolean sendMsg(String msg){
        try {
            Socket socketClient = new Socket();
            socketClient.connect(new InetSocketAddress(host, portOpponent), timeout);
            BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(socketClient.getOutputStream(), encoding));
            bw.write(String.format("%s", msg));
            bw.flush();
            socketClient.close();
            return true;
        } catch (ConnectException ex) {
            JOptionPane.showMessageDialog(null,
                    String.format("与服务器(host:%s, port:%d)连接失败，请重试", host, portOpponent), "提示", JOptionPane.ERROR_MESSAGE);
            return false;
        } catch (IOException ex){
            JOptionPane.showMessageDialog(null, String.format("与服务器(host:%s, port:%d)连接时发生IOException，请重试，请重试", host, portOpponent), "提示", JOptionPane.ERROR_MESSAGE);
            return false;
        }
    }

    public String getMsg(){
        try {
            Socket socketOpponent = server.accept();
            BufferedReader br = new BufferedReader(new InputStreamReader(socketOpponent.getInputStream(), encoding));
            StringBuilder sb = new StringBuilder();
            String msg = br.readLine();
            while(msg != null){
                sb.append(msg).append("\n");
                msg = br.readLine();
            }
            msg = sb.toString().trim();
            socketOpponent.close();
            if(msg.equals("CLOSE")){
                server.close();
            }
            return msg;
        } catch (Exception ex) {
            MainWindow.processException(null, ex);
            return null;
        }
    }

    public void destroy(){
        try {
            Socket socketClient = new Socket(host, portLocal);
            BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(socketClient.getOutputStream(), encoding));
            bw.write("CLOSE\n");
            bw.flush();
            socketClient.close();
        } catch (Exception ex) {
            MainWindow.processException(null, ex);
        }
    }
}
