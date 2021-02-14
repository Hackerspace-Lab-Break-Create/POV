package org.letbethelight.povdisplay.Service;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;


public class MessageService {

    private Map<String, Integer[]> lettersMap = new HashMap<>();

    private String message = "TOFUU";

    public MessageService() {

        for (Alphabet letter : Alphabet.values()) {

            lettersMap.put(letter.getLetter(), letter.getBinary());

        }
    }

    public String getWord() {

        return message;

    }

    public void setMessage(String message) {
        this.message = message;
    }


}
