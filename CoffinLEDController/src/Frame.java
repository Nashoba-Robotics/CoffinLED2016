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

    JLabel status;
    JPanel statusPanel;

    JPanel mainPanel;


    private Frame()
    {
        setLayout(new BorderLayout());

        status = new JLabel("Not Connected to Coffin");
        status.setForeground(Color.RED);
        status.setFont(new Font("Arial", Font.PLAIN, 20));
        statusPanel = new JPanel();
        statusPanel.add(status);

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
                    status.setText("Connected to Coffin");
                    status.setForeground(new Color(33, 150, 21));
                }
                else
                {
                    status.setText("Not Connected to Coffin");
                    status.setForeground(Color.RED);
                }
            }
        });
        Arduino.getInstance().initialize();

        setupNetworkListener();

        setSize(500, 300);
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
                    setTitle("Coffin Controller - Connected");
                }
                else
                {
                    setTitle("Coffin Controller - Disconnected");
                }
            }
        });
        Network.getInstance().setOnMessageReceivedListener(new Network.OnMessageReceivedListener()
        {
            @Override
            public void onMessageReceived(String key, Object value)
            {
                ArrayList<String> values = new ArrayList<String>();

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
