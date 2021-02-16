import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.websocket.OnClose;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

/**
 * @ServerEndpoint gives the relative name for the end point
 * This will be accessed via ws://localhost:8080/povdisplay
 */
@ServerEndpoint("/message")
public class WebSocketServer {

    private Map<Integer, String> words;
    private int counter = 1;

    @OnOpen
    public void onOpen(Session session){
        System.out.println(session.getId() + " has opened a connection");
        try {
            session.getBasicRemote().sendText("Connection Established");
            words = new HashMap<>();
            words.put(1,"TOFU");
            words.put(2,"KEVIN");
            words.put(3,"HACKERSPACE");
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    @OnMessage
    public void onMessage(String message, Session session){
        System.out.println("Message from " + session.getId() + ": " + message);
        //TODO implement code of updating word
        try {
            if(counter > words.size()){
                counter = 1;
            }
            session.getBasicRemote().sendText(words.get(counter) +"\n");
            counter++;
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    @OnClose
    public void onClose(Session session){
        System.out.println("Session " +session.getId()+" has ended");
    }
}