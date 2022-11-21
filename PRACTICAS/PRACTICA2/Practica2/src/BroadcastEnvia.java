import javax.swing.JFileChooser;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.io.*;
import java.net.*;
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
                FileInputStream objFIS = new FileInputStream(objF);
                BufferedInputStream objbis = new BufferedInputStream(objFIS);
                byte buffer[] = new byte[200];
                int aux = 0;
                String lineas = null;
                while((aux = objbis.read(buffer)) != -1){
                    lineas = new String(buffer, 0, aux);
                    DatagramPacket objDP = new DatagramPacket(lineas.getBytes(), lineas.length(), objIA, 9797);
                    objDS.send(objDP);
                    System.out.println(lineas);
                }
                objFIS.close();
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