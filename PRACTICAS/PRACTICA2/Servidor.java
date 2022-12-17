import java.io.*;
import java.net.*;
public class Servidor{
    public static void main(String[] args) throws Exception{
        Socket objS = null;
        ServerSocket objSS = new ServerSocket(8080);
        while(true){
            try{
                objS = objSS.accept();
                (new Ejecucion(objS)).start();
            }catch(Exception e){e.printStackTrace();}
        }
    }
    static class Ejecucion extends Thread{
        private Socket objS = null;
        private DataInputStream objOIS = null;;
        public Ejecucion(Socket objS){this.objS = objS;}
        public void run(){
            try{
                objOIS = new DataInputStream(objS.getInputStream());
                String nombre = objOIS.readUTF();
                long tam = objOIS.readLong();
                String direccion = "DOCUMENTOS/" + nombre;
                DataOutputStream objDOS = new DataOutputStream(new FileOutputStream(direccion));
                long recibido = 0;
                int a = 0, porciento = 0;
                byte[] paquete =  new byte[200];
                while(recibido < tam){
                    a = objOIS.read(paquete);
                    objDOS.write(paquete, 0, a);
                    objDOS.flush();
                    recibido += a;
                    porciento = (int)((recibido * 100) / tam);
                    System.out.println("Se ha recibido el " +porciento+ "%");
                }
                System.out.println(nombre);
                objDOS.close();
            }catch(Exception e){e.printStackTrace();}
            finally{
                try{
                    if(objOIS != null){objOIS.close();}
                    if(objS != null){objS.close();}
                }catch(Exception ex){ex.printStackTrace();}
            }
        }
    }
}