import java.io.*;
import java.net.*;
import javax.swing.JFileChooser;
public class Cliente{
    public static void main(String[] args) throws Exception{
        DataOutputStream objOOS = null;
        Socket objS = null;
        try{
            objS = new Socket("127.0.0.1", 8080);
            objOOS = new DataOutputStream(objS.getOutputStream());
            JFileChooser objJFC = new JFileChooser();
            File archivoSeleccionado;
            while(true){
                int res = objJFC.showOpenDialog(null);
                if(res == JFileChooser.APPROVE_OPTION){
                    archivoSeleccionado = objJFC.getSelectedFile();
                    String nombre = archivoSeleccionado.getName();
                    String ruta = archivoSeleccionado.getAbsolutePath();
                    long tam = archivoSeleccionado.length();
                    DataInputStream objDIS = new DataInputStream(new FileInputStream(ruta));
                    objOOS.writeUTF(nombre);
                    objOOS.flush();
                    objOOS.writeLong(tam);
                    objOOS.flush();
                    long enviado = 0;
                    int a = 0, porcentaje = 0;
                    byte[] paquete = new byte[200];
                    while(enviado < tam){
                        a = objDIS.read(paquete);
                        objOOS.write(paquete, 0, a);
                        objOOS.flush();
                        enviado += a;
                        porcentaje = (int)((enviado * 100) / tam);
                        System.out.println("Enviado el "+porcentaje+"%");
                    }
                    System.out.println("Se ha subido el archivo");
                    objDIS.close();
                }else{break;}
            }
        }catch(Exception e){e.printStackTrace();}
        finally{
            if(objOOS != null){objOOS.close();}
            if(objS != null){objS.close();}
        }
    }
}