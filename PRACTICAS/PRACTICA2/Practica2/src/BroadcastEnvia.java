import java.io.File;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

import javax.swing.JFileChooser;
import javax.swing.filechooser.FileNameExtensionFilter;
public class BroadcastEnvia{
    public static void main(String[] args) throws Exception{
        final InetAddress objIA = InetAddress.getByName("224.0.0.1");
        final DatagramSocket objDS = new DatagramSocket();
        FileNameExtensionFilter objFNEF = new FileNameExtensionFilter(".bmp, .png, .jpg, .txt", "bmp", "png", "jpg", "txt");
        JFileChooser objJFC = new JFileChooser();
        while(true){
            objJFC.setFileFilter(objFNEF);
            int opcion = objJFC.showOpenDialog(null);
            if(opcion == JFileChooser.APPROVE_OPTION){
                File objF = objJFC.getSelectedFile();
                byte[] buffer = objF.getName().getBytes();
                DatagramPacket objDP = new DatagramPacket(buffer, buffer.length, objIA, 9797);
                objDS.send(objDP);
            }else{
                String salir = "SALIR";
                byte[] buffer = salir.getBytes();
                DatagramPacket objDP = new DatagramPacket(buffer, buffer.length, objIA, 9797);
                objDS.send(objDP);
                break;
            }
        }
        objDS.close();
    }
}