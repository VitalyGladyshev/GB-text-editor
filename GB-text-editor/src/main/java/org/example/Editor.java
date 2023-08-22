package org.example;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class Editor extends JFrame implements ActionListener {
    private JTextArea jTextArea;
    private JScrollPane jScrollPane;
    private JMenuBar jMenuBar;
    private JMenu jMenu;
    private JMenuItem openItem;
    private JMenuItem saveItem;
    private JMenuItem exitItem;
    public Editor() {
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setTitle("Editor");
        this.setSize(500,500);
        this.setLayout(new FlowLayout());
        this.setLocationRelativeTo(null);
        jTextArea = new JTextArea();
        jTextArea.setLineWrap(true);
        jTextArea.setWrapStyleWord(true);
        jTextArea.setFont(new Font("Arial",Font.PLAIN ,20));

        jScrollPane = new JScrollPane(jTextArea);
        jScrollPane.setPreferredSize(new Dimension(450,450));
        jScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);

        jMenuBar = new JMenuBar();
        jMenu = new JMenu("File");
        openItem = new JMenuItem("Open");
        saveItem = new JMenuItem("Save");
        exitItem = new JMenuItem("Exit");
        openItem.addActionListener(this);
        saveItem.addActionListener(this);
        exitItem.addActionListener(this);
        jMenu.add(openItem);
        jMenu.add(saveItem);
        jMenu.add(exitItem);
        jMenuBar.add(jMenu);

        this.setJMenuBar(jMenuBar);
        this.add(jScrollPane);
        this.setVisible(true);

    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getSource()==openItem){
            JFileChooser jFileChooser = new JFileChooser();
            jFileChooser.setCurrentDirectory(new File("."));
            FileNameExtensionFilter fileNameExtensionFilter = new FileNameExtensionFilter("Text files","txt");
            jFileChooser.setFileFilter(fileNameExtensionFilter);
            int response = jFileChooser.showOpenDialog(null);
            if (response == JFileChooser.APPROVE_OPTION){
                File file = new File(jFileChooser.getSelectedFile().getAbsolutePath());
                Scanner sc = null;
                try {
                    sc = new Scanner(file);
                    if (file.isFile()){
                        while (sc.hasNextLine()){
                            String line = sc.nextLine()+"\n";
                            jTextArea.append(line);
                        }
                    }
                } catch (FileNotFoundException ex) {
                    ex.printStackTrace();
                }
                finally {
                    sc.close();
                }
            }
        }
        if(e.getSource()==saveItem){
            JFileChooser jFileChooser = new JFileChooser();
            jFileChooser.setCurrentDirectory(new File("."));
            int response = jFileChooser.showSaveDialog(null);
            if(response == JFileChooser.APPROVE_OPTION){
                File file;
                PrintWriter printWriter = null;
                file = new File(jFileChooser.getSelectedFile().getAbsolutePath());
                try {
                    printWriter = new PrintWriter(file);
                    printWriter.println(jTextArea.getText());
                } catch (FileNotFoundException ex) {
                    ex.printStackTrace();
                }
                finally {
                    printWriter.close();
                }
            }
        }
        if(e.getSource()==exitItem){
            System.exit(0);
        }
    }
}
