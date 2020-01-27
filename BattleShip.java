package battleship;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import javax.swing.*;
import java.awt.Color;

public class GameBoard extends JFrame implements KeyListener{
    private final int rows = 8; //the number of rows
    private final int cols = 8; //the number of columns
    private final int gap = 2; //a gap to space out the game board
    private int currentX = 4; //starting x position
    private int currentY = 4; //starting y position
    private int turn = 50; //how many turns you get to win
    private int winCount = 0; //determines when you win
    
    int dice1 = 0, dice2 = 0; //random dice used later
    
    private JPanel right = new JPanel(); //panel on the right of the main game board
    private JPanel left = new JPanel(); //panel on the left of the main game board
    private JPanel winTop = new JPanel(); //panel at the top of the winner panel
    private JPanel winMid = new JPanel(); //panel in the middle of the winner panel
    private JPanel loseTop = new JPanel(); //panel at the top of the loser panel
    private JPanel loseMid = new JPanel(); //panel in the middle of the loser panel
    
    //5 images that display the logo, the image when you get a hit, the image when you miss, when you win and when you lose
    ImageIcon logo = new ImageIcon("C:\\Users\\rylol\\OneDrive\\Pictures\\CPSC FInal Project Pictures\\Logo.png");
    ImageIcon hit = new ImageIcon("C:\\Users\\rylol\\OneDrive\\Pictures\\CPSC FInal Project Pictures\\ShipExplode.png");
    ImageIcon miss = new ImageIcon("C:\\Users\\rylol\\OneDrive\\Pictures\\CPSC FInal Project Pictures\\ShipMiss.png");
    ImageIcon winPic = new ImageIcon("C:\\Users\\rylol\\OneDrive\\Pictures\\CPSC FInal Project Pictures\\shipWin.jpg");
    ImageIcon lossPic = new ImageIcon("C:\\Users\\rylol\\OneDrive\\Pictures\\CPSC FInal Project Pictures\\ShipExplode.jpg");
    Image transformed;
    Image transformed2;
    Image transformed3;
    Image transformed4;
    Image transformed5;
    
    private JPanel main = new JPanel(new BorderLayout()); //main panel that displays the game
    private JPanel win = new JPanel(new BorderLayout()); //panel that displays when you win
    private JPanel loss = new JPanel(new BorderLayout()); //panel that displays when you lose
    private JLabel message = new JLabel("        Find the ships!               ");//message that gives info
    private JLabel title = new JLabel(); //label that the picture is on
    private JLabel pic = new JLabel(); //label that displays the hit or miss pictures
    private JLabel winnerPic = new JLabel(); //label that displays the picture when you win
    private JLabel loserPic = new JLabel(); //label that displays the picture when you lose
    private JLabel turns = new JLabel("        " + turn + " turns left               "); //label that displays the # of turns
    private JLabel winMes = new JLabel("YOU WIN!!!!"); //label that displays the winning message
    private JLabel loseMes = new JLabel("YOU LOSE!!!!"); //label that displays the losing message
    private JPanel ocean = new JPanel(new GridLayout(rows,cols,gap,gap)); //panel that displays the gameboard
    private JPanel[][] panel = new JPanel[rows][cols]; //2D array of panels which make up the game board
    private int[][] ships = new int[rows][cols]; //2D array of integers to determine if a ship is there or not
    private Color [][] boardColor = new Color[rows][cols]; //gives the game board a color
    
    CardLayout cardLayout = new CardLayout(); //cardlayout so you can switch panels
    
    public GameBoard() //constructor
    {
        super("BATTLESHIP"); //gives the panel a title
        setSize(1000,1000); //sets the size of a panel
        setLocationRelativeTo(null); //sets the location of the panel in the middle of the screen
        setDefaultCloseOperation(EXIT_ON_CLOSE); //you can close the operation using the exit button
        
        addKeyListener(this); //adds the key listener to the panels
        
        setLayout(cardLayout); //sets the cardlayout 
        
        left.setLayout(new GridLayout(1,1,0,0)); //sets the left panel's layout to gridlayout 
        right.setLayout(new GridLayout(3,1,0,0)); //sets the right panel's layout to gridlayout
        
        //for loop that creates the ocean game board
        for(int i = 0; i < rows; i++)
        {
            for(int x = 0; x < cols; x++)
            {
                panel[i][x] = new JPanel();
                ships[i][x] = 0; //makes it so there is no ships yet
                ocean.add(panel[i][x]); //adds a panel to each ocean space
                boardColor[i][x] = Color.BLUE; //sets the color to blue
                panel[i][x].setBackground(boardColor[i][x]);
            }
        }
        //for loop that creates the 10 random ship positions
        for(int y = 0; y < 10; y++)
        {
            dice1 = (int)(Math.random()*7) + 1;
            dice2 = (int)(Math.random()*7) + 1;
            ships[dice1][dice2] = 1;
        }

        
        panel[currentX][currentY].setBackground(Color.BLACK); //sets the cursor to black
        
        //adds the panels to the Pane
        add(main, "Panel 1");
        add(win, "Panel 2");
        add(loss, "Panel 3");
        
        //sets the fonts of all the messages
        message.setFont(new Font("Arial", Font.BOLD, 20));
        turns.setFont(new Font("Arial", Font.BOLD, 20));
        winMes.setFont(new Font("Arial", Font.BOLD, 40));
        loseMes.setFont(new Font("Arial", Font.BOLD, 40));
        
        //converts all of the ImageIcons to Images and scales them
        transformed = logo.getImage().getScaledInstance(950,100,java.awt.Image.SCALE_SMOOTH);
        logo = new ImageIcon(transformed);
        
        transformed2 = hit.getImage().getScaledInstance(300,300,java.awt.Image.SCALE_SMOOTH);
        hit = new ImageIcon(transformed2);
        
        transformed3 = miss.getImage().getScaledInstance(300,300,java.awt.Image.SCALE_SMOOTH);
        miss = new ImageIcon(transformed3);
        
        transformed4 = winPic.getImage().getScaledInstance(700,700,java.awt.Image.SCALE_SMOOTH);
        winPic = new ImageIcon(transformed4);
        
        transformed5 = lossPic.getImage().getScaledInstance(700,700,java.awt.Image.SCALE_SMOOTH);
        lossPic = new ImageIcon(transformed5);
        
        //sets the background of the right panel and adds a picture and a message to it
        right.setBackground(Color.YELLOW);
        right.add(pic);
        right.add(message);
        
        //sets the background of the left panel and adds the turn counter to it
        left.setBackground(Color.YELLOW);
        left.add(turns);
        
        //adds the game board, the title picture and the left and right panels to the main panel
        main.add(ocean, BorderLayout.CENTER);
        main.add(title, BorderLayout.NORTH);
        title.setIcon(logo);
        main.add(right, BorderLayout.EAST);
        main.add(left, BorderLayout.WEST);
        
        //adds the winning picture and the message to the winning panel and sets its background
        winMid.add(winnerPic);
        winMid.setBackground(Color.GREEN);
        winTop.add(winMes);
        winTop.setBackground(Color.GREEN);
        win.add(winMid, BorderLayout.CENTER);
        win.add(winTop, BorderLayout.NORTH);
        
        //adds the losing picture and the message to the losing panel and sets its background 
        loseMid.add(loserPic);
        loseMid.setBackground(Color.RED);
        loseTop.add(loseMes);
        loseTop.setBackground(Color.RED);
        loss.add(loseMid, BorderLayout.CENTER);
        loss.add(loseTop, BorderLayout.NORTH);  
    }
    //method that determines if the space is a ship or not
    public boolean isShip()
    {
        if(ships[currentX][currentY] == 1)
            return true;
        else
            return false;
    }
    //method that determines if you won the game
    public boolean isWin(int count)
    {
        if(count == 10)
            return true;
        else
            return false;      
    }
     
    @Override
    public void keyTyped(KeyEvent e) {
        
    }

    @Override
    public void keyPressed(KeyEvent e) {
        int keyCode = e.getKeyCode();
        
        if(keyCode == KeyEvent.VK_UP) //makes it so you move up when you press the up key
        {
            if(currentX - 1 >= 0)
            {
                panel[currentX][currentY].setBackground(boardColor[currentX][currentY]);
                currentX = currentX - 1;
                panel[currentX][currentY].setBackground(Color.BLACK);
            }
        }
        if(keyCode == KeyEvent.VK_DOWN) //makes it so you move down when you press the down key
        {
            if(currentX + 1 <= 7)
            {
                panel[currentX][currentY].setBackground(boardColor[currentX][currentY]);
                currentX = currentX + 1;
                panel[currentX][currentY].setBackground(Color.BLACK);
            }
        }
        if(keyCode == KeyEvent.VK_LEFT) // makes it so you move left when you press the left key
        {
            if(currentY - 1 >= 0)
            {
                panel[currentX][currentY].setBackground(boardColor[currentX][currentY]);
                currentY = currentY - 1;
                panel[currentX][currentY].setBackground(Color.BLACK);
            }
        }
        if(keyCode == KeyEvent.VK_RIGHT) //makes it so you move right whrn you press the right key
        {
            if(currentY + 1 <= 7)
            {
                panel[currentX][currentY].setBackground(boardColor[currentX][currentY]);
                currentY = currentY + 1;
                panel[currentX][currentY].setBackground(Color.BLACK);
            }
        }
        if(keyCode == KeyEvent.VK_SPACE)
        {
           if(turn > 0) //if you still have turns
           {    
                if(isShip() == true) //if there is a ship in the spot you selected
                {
                    winCount++; //increase the counter that makes you win once it hits 10
                    boardColor[currentX][currentY] = Color.GREEN; //sets the current spot to green
                    turn = turn - 1; //decrease the turn counter
                    turns.setText("               " + turn + " turns left                      "); //updates the turn counter
                    message.setText("               Hit!                     "); //displays that it was a hit
                    pic.setIcon(hit); //changes the picture to the hit picture
                    right.setBackground(Color.GREEN); //sets the background to green
                    left.setBackground(Color.GREEN); //sets the background to green
                    if(isWin(winCount) == true) //if you got all the ships
                    {    
                        cardLayout.show(getContentPane(), "Panel 2"); //display the winner panel
                        winnerPic.setIcon(winPic); //display the winning picture
                    }    
                }
                else //if there wasn't a ship in the spot you selected
                {
                    boardColor[currentX][currentY] = Color.RED; //sets the current spot to red
                    turn = turn - 1; //decreases the turn counter
                    turns.setText("               " + turn + " turns left                      "); //updates the turn counter
                    message.setText("               Miss!                     "); //displays that it was a miss
                    pic.setIcon(miss); //sets the picture to the miss picture
                    right.setBackground(Color.RED); //sets the background to red
                    left.setBackground(Color.RED); //sets the background to red
                }
           }
           else //if there is not more turns
           {    
               cardLayout.show(getContentPane(), "Panel 3"); //display the losing panel
               loserPic.setIcon(lossPic); //displays the losing picture
           }
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
}




