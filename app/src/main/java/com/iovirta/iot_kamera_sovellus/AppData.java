package com.iovirta.iot_kamera_sovellus;

/**
 * Created by rytai on 15.3.2017.
 */

public class AppData {
}
    private static AppData mInstance= null;

    public int someValueIWantToKeep;

    protected AppData(){}

    public static synchronized MyProperties getInstance(){
        if(null == mInstance){
            mInstance = new MyProperties();
        }
        return mInstance;
    }
}