import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

/**
 * Created by Colin on 4/3/2015.
 */
public class Frame extends JFrame implements WindowListener
{
    static final Object lock = new Object();
    private static Frame singleton;
    public static Frame getInstance()
    {
        synchronized (lock)
        {
            if(singleton == null)
                singleton = new Frame();
            return singleton;
        }
    }

    JLabel coffinStatus;
    JLabel networkStatus;
    JPanel statusPanel;

    JPanel mainPanel;


    private Frame()
    {
        setLayout(new BorderLayout());

        coffinStatus = new JLabel("Not Connected to Coffin");
        coffinStatus.setForeground(Color.RED);
        coffinStatus.setFont(new Font("Arial", Font.PLAIN, 20));

        networkStatus = new JLabel("Not Connected to Robot");
        networkStatus.setForeground(Color.RED);
        networkStatus.setFont(new Font("Arial", Font.PLAIN, 20));

        statusPanel = new JPanel(new GridLayout(2, 1));
        statusPanel.add(coffinStatus);
        statusPanel.add(networkStatus);

        add(statusPanel, BorderLayout.NORTH);

        mainPanel = new JPanel(new GridLayout(2, 1));

        mainPanel.setBorder(new EmptyBorder(20, 20, 20, 20));
        add(mainPanel, BorderLayout.CENTER);

        Arduino.getInstance().setConnectionStatusListener(new Arduino.ConnectionStatusChangeListener()
        {
            @Override
            public void statusChanged()
            {
                if (Arduino.getInstance().getConnectionStatus())
                {
                    coffinStatus.setText("Connected to Coffin");
                    coffinStatus.setForeground(new Color(33, 150, 21));
                }
                else
                {
                    coffinStatus.setText("Not Connected to Coffin");
                    coffinStatus.setForeground(Color.RED);
                }
            }
        });
        Arduino.getInstance().setMessageListener(new Arduino.MessageListener()
        {
            @Override
            public void onMessageReceived(String message)
            {
                System.out.println(message);
            }
        });
        Arduino.getInstance().initialize();

        setupNetworkListener();

        setSize(1000, 600);
        addWindowListener(this);
        setTitle("Coffin Controller");
        setLocationRelativeTo(null);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setVisible(true);
    }


    private void setupNetworkListener()
    {
        Network.getInstance().setConnectionListener(new Network.ConnectionListener()
        {
            @Override
            public void onConnectionStateChanged(boolean state)
            {
                if(state)
                {
                    networkStatus.setText("Connected to Robot");
                    networkStatus.setForeground(new Color(33, 150, 21));
                }
                else
                {
                    networkStatus.setText("Not Connected to Robot");
                    networkStatus.setForeground(Color.RED);
                }
            }
        });
        Network.getInstance().setOnMessageReceivedListener(new Network.OnMessageReceivedListener()
        {
            @Override
            public void onMessageReceived(String key, Object value)
            {
                ArrayList<Boolean> values = new ArrayList<Boolean>();
                values.add(Network.getInstance().getBoolean("Hood Bottom"));
                values.add(Network.getInstance().getBoolean("Hood Top"));
                values.add(Network.getInstance().getBoolean("Hood at Threshold"));

                values.add(Network.getInstance().getBoolean("Intake Top Stop"));
                values.add(Network.getInstance().getBoolean("Intake Top"));
                values.add(Network.getInstance().getBoolean("Intake Top Intake"));
                values.add(Network.getInstance().getBoolean("Intake Intake Home"));
                values.add(Network.getInstance().getBoolean("Intake Home"));
                values.add(Network.getInstance().getBoolean("Intake Home Bottom"));
                values.add(Network.getInstance().getBoolean("Intake Bottom"));
                values.add(Network.getInstance().getBoolean("Intake Bottom Stop"));

                values.add(Network.getInstance().getBoolean("Photo 1"));
                values.add(Network.getInstance().getBoolean("Photo 2"));
                values.add(Network.getInstance().getBoolean("Photo 3"));

                values.add(Network.getInstance().getBoolean("Elevator Bottom Height"));
                values.add(Network.getInstance().getBoolean("Elevator Top Height"));
                values.add(Network.getInstance().getBoolean("Elevator Motor Running"));

                StringBuilder build = new StringBuilder();
                for(int i = 0; i < values.size(); i++)
                {
                    if(values.get(i) != null)
                        build.append(values.get(i)? "1" : "0");
                    if(i != values.size()-1)
                        build.append(":");
                }

                //Send the individual LED data to the arduino
                //The '0' prefix tells the arduino what kind of data it's getting
                Arduino.getInstance().sendMessage("0" + build.toString());

                Boolean autoAligning = Network.getInstance().getBoolean("Auto Align Happening");
                Boolean allSystemsGo = Network.getInstance().getBoolean("All Systems Go");
                String message = "1";
                if(autoAligning != null)
                    message += autoAligning? "1" : "0";
                else
                    message += "0";
                if(allSystemsGo != null)
                    message += allSystemsGo? "1" : "0";
                else
                    message += "0";
                Arduino.getInstance().sendMessage(message);

            }
	});

        Network.getInstance().connect();
    }

    @Override
    public void windowOpened(WindowEvent e)
    {

    }

    @Override
    public void windowClosing(WindowEvent e)
    {
        System.out.println("Serial Closed");
        Arduino.getInstance().close();
        System.out.println("Serial Done Closing");
    }

    @Override
    public void windowClosed(WindowEvent e)
    {

    }

    @Override
    public void windowIconified(WindowEvent e)
    {

    }

    @Override
    public void windowDeiconified(WindowEvent e)
    {

    }

    @Override
    public void windowActivated(WindowEvent e)
    {

    }

    @Override
    public void windowDeactivated(WindowEvent e)
    {

    }
}
