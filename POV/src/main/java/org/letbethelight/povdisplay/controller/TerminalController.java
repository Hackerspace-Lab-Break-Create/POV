package org.letbethelight.povdisplay.controller;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

import com.fazecast.jSerialComm.SerialPort;
import org.letbethelight.povdisplay.Service.MessageService;


public class TerminalController {

    private ServerSocket sp;
    private MessageService messageService;
    private BufferedWriter bw;
    private Socket arduino;

    public TerminalController() {
        try {
            sp = new ServerSocket(9090);
            arduino = sp.accept();
        } catch (IOException e) {
            e.printStackTrace();
        }
        messageService = new MessageService();
    }

    public void init() {
        try {

            System.out.println("Set the POV word: \n");
            arduino.setKeepAlive(true);
            bw = new BufferedWriter(new OutputStreamWriter(arduino.getOutputStream()));
            Scanner sc = new Scanner(System.in);
            bw.write(sc.nextLine() + "\n");
            bw.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public void closeConnections(){
        try {
            arduino.close();
            sp.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
