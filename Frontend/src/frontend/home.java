/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package frontend;

import java.awt.Color;
import java.awt.Desktop;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFileChooser;
import javax.swing.filechooser.FileNameExtensionFilter;

/**
 *
 * @author ghost_000
 */
public class home extends javax.swing.JFrame {

    int isfilepicked = 0;
    FileNameExtensionFilter filterpdf = new FileNameExtensionFilter("PDF Documents", "pdf");
    FileNameExtensionFilter filterimage = new FileNameExtensionFilter("Images", "jpeg", "jpg", "png");
    String RESOLUTION = "300";
    String selectedFile = "";
    /**
     * Creates new form home
     */
    public home() {
        
        initComponents();
        this.pack();
        this.setLocationRelativeTo(null);
        filename.setVisible(false);
        loadergif.setVisible(false);
        File homedir = new File(System.getProperty("user.home"));
        File currdir = new File(System.getProperty("user.dir"));
        pickpdf.setCurrentDirectory(homedir);
        pickpdf.setFileFilter(filterpdf);
        pickimage.setCurrentDirectory(homedir);
        pickimage.setFileFilter(filterimage);
        pickpdf.setMultiSelectionEnabled(true);
        pickimage.setMultiSelectionEnabled(true);
        pdf.addMouseListener(new MouseAdapter()  
        {  
            public void mouseClicked(MouseEvent e)  
            {
                if(pickpdf.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
                    isfilepicked = 1;
                    File[] files = pickpdf.getSelectedFiles();
                    System.err.println("Number of files " + files.length);
                    String fileList = "";
                    for (File file : files) {
                        fileList = fileList.concat(file.getAbsolutePath() + " ");
                        System.err.println("File name " + file.getAbsolutePath());
                    }
                    System.err.println(fileList);
                    try {
                        String cmd = "python ../Backend/pdf2png.py " 
                                + RESOLUTION + " " + fileList;
                        System.err.println(cmd);
                        Process p = Runtime.getRuntime().exec(cmd);
                        BufferedReader in = new BufferedReader(
                                new InputStreamReader(p.getErrorStream()) );
                        String line;
                        while ((line = in.readLine()) != null) {
                            System.err.println(line);
                        }
                        in.close();
                        p.waitFor();
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    } catch (InterruptedException ex) {
                        ex.printStackTrace();
                    }
                    filename.setText("Uploaded: " + pickpdf.getSelectedFile());
                    filename.setVisible(true);
                }
            }  
        });
        image.addMouseListener(new MouseAdapter()  
        {  
            public void mouseClicked(MouseEvent e)  
            {
                if(pickimage.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
                    isfilepicked = 1;
                    File[] files = pickimage.getSelectedFiles();
                    System.err.println("Number of files " + files.length);
                    for (File file : files) {
                        System.err.println("File name " + file.getAbsolutePath());
                        try {
                            String cmd = "cp " + file + " ../Images/Input/";
                            Process p = Runtime.getRuntime().exec(cmd);
                            p.waitFor();
                        } catch (IOException ex) {
                            ex.printStackTrace();
                        } catch (InterruptedException ex) {
                            ex.printStackTrace();
                        }
                    }
                    filename.setText("Uploaded: " + pickimage.getSelectedFile());
                    filename.setVisible(true);
                }
            }  
        });
        convert.addMouseListener(new MouseAdapter()  
        {  
            public void mouseClicked(MouseEvent e)  
            {
                if (isfilepicked !=0)
                    loadergif.setVisible(true);
                
                if (isfilepicked != 0) {
                    loadPlots();
                    loadColorsAndLegends();
                    generateScalesAndData();
                    generateOutput();
                } 
                else {
                    filename.setText("Pick a file first!");
                    filename.setVisible(true);
                }
            }  
        });
        report.addMouseListener(new MouseAdapter()  
        {  
            public void mouseClicked(MouseEvent e)  
            {
                Desktop desktop = Desktop.getDesktop();
                if (desktop.isSupported(Desktop.Action.OPEN)) {
                    try {
                        desktop.open(new File("opensoft-report.pdf"));
                    } catch (IOException ex) {
                        Logger.getLogger(home.class.getName()).log(Level.SEVERE, null, ex);
                    }
                } else {
                    System.out.println("Open is not supported");
                }
            }  
        });
    }
    
    void loadPlots() {
        String[] cmd = {"sh", "-c", 
            "cd ../Backend/graph_extractor && ./opensoft"};
        System.err.println(Arrays.deepToString(cmd));
        executeCommandSh(cmd);
//        try {
//            Process p = Runtime.getRuntime().exec(cmd);
//            BufferedReader in = new BufferedReader(
//                    new InputStreamReader(p.getErrorStream()) );
//            String line;
//            while ((line = in.readLine()) != null) {
//                System.err.println(line);
//            }
//            in.close();
//            System.err.println(Arrays.deepToString(cmd));
//            p.waitFor();
//            System.err.println("Plots loaded");
//        } catch (IOException ex) {
//            ex.printStackTrace();
//        } catch (InterruptedException ex) {
//            ex.printStackTrace();
//        }
    }
    
    void loadColorsAndLegends() {
        File home = new File("../Backend/graph_extractor");
        String[] dirs = home.list();
        
        for(String dir : dirs) {
            if (dir.startsWith("test_") && 
                    new File("../Backend/graph_extractor/" + dir).isDirectory()) {
                String[] subdirs = new File("../Backend/graph_extractor/" + dir).list();
                for (String subdir : subdirs) {
                    if (subdir.startsWith("graph_") &&
                            new File("../Backend/graph_extractor/" + dir + "/" + subdir).isDirectory()) {
                        String[] files = new File("../Backend/graph_extractor/" + dir + "/" + subdir).list();
                        for (String file : files) if (file.endsWith(".png") && file.startsWith("graph_")){
                            String base = file.split("\\.")[0];
                            System.err.println("base : " + base);
                            String pref = "graph_extractor/" + dir + "/" + subdir + "/";
                            String[] cmd = {"sh", "-c", ""};
                            
                            cmd[2] = "cd ../Backend && ./legend_detection "
                                    + pref + base + ".png "
                                    + pref + base + ".txt "
                                    + pref + base + "_legend.txt";
                            System.err.println(Arrays.deepToString(cmd));
                            executeCommandSh(cmd);

                            cmd[2] = "cd ../Backend/graph_extractor/" + dir + "/" + subdir + " && "
                                    + "../../../separate_colors " + base + ".png " + base + ".txt "
                                    + base + "_legend.txt";

                            System.err.println(Arrays.deepToString(cmd));
                            executeCommandSh(cmd);

                            cmd[2] = "cd ../Backend/graph_extractor/" + dir + "/" + subdir + " && "
                                    + "../../../match_legend " + base + ".png " + base + "_legend.txt "
                                    + "colors.txt";
                            System.err.println(Arrays.deepToString(cmd));
                            executeCommandSh(cmd);
                        }
                    }
                }
            }
        }
        System.err.println("Legends added");
    }
    
    void generateScalesAndData() {
        File home = new File("../Backend/graph_extractor");
        String[] dirs = home.list();
        
        for(String dir : dirs) {
            if (dir.startsWith("test_") && 
                    new File("../Backend/graph_extractor/" + dir).isDirectory()) {
                String[] subdirs = new File("../Backend/graph_extractor/" + dir).list();
                for (String subdir : subdirs) {
                    if (subdir.startsWith("graph_") &&
                            new File("../Backend/graph_extractor/" + dir + "/" + subdir).isDirectory()) {
                        String pref = new File("../Backend/graph_extractor/" + dir + "/" + subdir).getAbsolutePath();
                        String[] files = new File("../Backend/graph_extractor/" + dir + "/" + subdir).list();
                        for (String file : files) if (file.endsWith(".png") && file.startsWith("graph_")){
                            String base = file.split("\\.")[0];
                            System.err.println("base : " + base);
                            String[] cmd = {"sh", "-c", ""};
                            cmd[2] = "cd ../Backend/graph_extractor/" + dir + "/" + subdir + " && "
                                    + "sh ../../../scaledetection/findaxis.sh " + base + ".png " + base + ".txt ";

                            executeCommandSh(cmd);
                            
                            cmd[2] = "cd ../Backend/graph_extractor/" + dir + "/" + subdir + " && "
                                    + "../../../testx";
                            executeCommandSh(cmd);
                            
                            cmd[2] = "cd ../Backend/graph_extractor/" + dir + "/" + subdir + " && "
                                    + "../../../testy";
                            executeCommandSh(cmd);
                            
                            cmd[2] = "cd ../Backend/graph_extractor/" + dir + "/" + subdir + " && "
                                    + "python ../../../plotpoints.py";
                            executeCommandSh(cmd);
                            
                            cmd[2] = "cd ../Backend/graph_extractor/" + dir + "/" + subdir + " && "
                                    + "python ../../../rearrange.py";
                            executeCommandSh(cmd);
                            
                            cmd[2] = "cd ../Backend/graph_extractor/" + dir + "/" + subdir + " && "
                                    + "python ../../../create_output.py";
                            executeCommandSh(cmd);
                        }
                        
                    }
                }
            }
        }
        System.err.println("Output creation completed");
    }
    
    void generateOutput() {
        String[] cmd = {"sh", "-c", "cd ../Backend && cd ../Backend && python ../Backend/htmlpdfgen.py"};
        executeCommandSh(cmd);
        Desktop desktop = Desktop.getDesktop();
        if (desktop.isSupported(Desktop.Action.OPEN)) {
            try {
                desktop.open(new File("../Backend/output.pdf"));
            } catch (IOException ex) {
                Logger.getLogger(home.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
    void executeCommandSh(String[] cmd) {
        try {
            System.err.println(Arrays.deepToString(cmd));

            Process p = Runtime.getRuntime().exec(cmd);
            BufferedReader in = new BufferedReader(
                    new InputStreamReader(p.getErrorStream()));
            String line;
            while ((line = in.readLine()) != null) {
                System.err.println(line);
            }
            in.close();
            p.waitFor();
            System.err.println("Execution complete");
        } catch (IOException ex) {
            ex.printStackTrace();
        } catch (InterruptedException ex) {
            ex.printStackTrace();
        }
    }
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        pickpdf = new javax.swing.JFileChooser();
        pickimage = new javax.swing.JFileChooser();
        background = new javax.swing.JPanel();
        foreground = new javax.swing.JPanel();
        title = new javax.swing.JLabel();
        hall = new javax.swing.JLabel();
        pdf = new javax.swing.JLabel();
        image = new javax.swing.JLabel();
        convert = new javax.swing.JLabel();
        filename = new javax.swing.JLabel();
        opensoft = new javax.swing.JLabel();
        report = new javax.swing.JLabel();
        loadergif = new javax.swing.JLabel();
        fg = new javax.swing.JLabel();
        bg = new javax.swing.JLabel();

        pickpdf.setDialogTitle("Choose your PDF");

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Welcome!");
        setMinimumSize(new java.awt.Dimension(1000, 750));
        setResizable(false);

        background.setMaximumSize(new java.awt.Dimension(1000, 750));
        background.setMinimumSize(new java.awt.Dimension(1000, 750));
        background.setPreferredSize(new java.awt.Dimension(1000, 750));
        background.setLayout(null);

        foreground.setBackground(new java.awt.Color(255, 255, 255));
        foreground.setMaximumSize(new java.awt.Dimension(900, 650));
        foreground.setMinimumSize(new java.awt.Dimension(900, 650));
        foreground.setPreferredSize(new java.awt.Dimension(900, 650));
        foreground.setLayout(null);

        title.setFont(new java.awt.Font("Tahoma", 1, 36)); // NOI18N
        title.setForeground(new java.awt.Color(255, 255, 255));
        title.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        title.setText("Plots to Tables");
        foreground.add(title);
        title.setBounds(235, 100, 450, 60);

        hall.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        hall.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        hall.setText("TEAM 4");
        foreground.add(hall);
        hall.setBounds(5, 600, 910, 30);

        pdf.setFont(new java.awt.Font("Tahoma", 1, 13)); // NOI18N
        pdf.setForeground(new java.awt.Color(255, 255, 255));
        pdf.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        pdf.setText("Upload PDF");
        pdf.setBorder(new javax.swing.border.LineBorder(new java.awt.Color(255, 255, 255), 1, true));
        foreground.add(pdf);
        pdf.setBounds(275, 250, 150, 30);

        image.setFont(new java.awt.Font("Tahoma", 1, 13)); // NOI18N
        image.setForeground(new java.awt.Color(255, 255, 255));
        image.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        image.setText("Upload Image");
        image.setBorder(new javax.swing.border.LineBorder(new java.awt.Color(255, 255, 255), 1, true));
        foreground.add(image);
        image.setBounds(500, 250, 150, 30);

        convert.setFont(new java.awt.Font("Tahoma", 1, 13)); // NOI18N
        convert.setForeground(new java.awt.Color(255, 255, 255));
        convert.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        convert.setText("CONVERT !");
        convert.setBorder(new javax.swing.border.LineBorder(new java.awt.Color(255, 255, 255), 1, true));
        foreground.add(convert);
        convert.setBounds(275, 325, 375, 30);

        filename.setForeground(new java.awt.Color(255, 255, 255));
        filename.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        filename.setText("Uploaded : ");
        foreground.add(filename);
        filename.setBounds(5, 290, 890, 30);

        opensoft.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        opensoft.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        opensoft.setText("OPENSOFT 2016");
        foreground.add(opensoft);
        opensoft.setBounds(5, 570, 910, 30);

        report.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        report.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        report.setText("DOCUMENTATION");
        report.setBorder(new javax.swing.border.LineBorder(new java.awt.Color(0, 0, 0), 1, true));
        foreground.add(report);
        report.setBounds(325, 530, 280, 30);

        loadergif.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        loadergif.setIcon(new javax.swing.ImageIcon(getClass().getResource("/frontend/ajax-loader.gif"))); // NOI18N
        foreground.add(loadergif);
        loadergif.setBounds(0, 360, 900, 130);

        fg.setIcon(new javax.swing.ImageIcon(getClass().getResource("/frontend/fg.png"))); // NOI18N
        foreground.add(fg);
        fg.setBounds(0, 0, 900, 650);

        background.add(foreground);
        foreground.setBounds(50, 50, 900, 650);

        bg.setIcon(new javax.swing.ImageIcon(getClass().getResource("/frontend/bgb.png"))); // NOI18N
        background.add(bg);
        bg.setBounds(0, 0, 1000, 750);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(background, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(background, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(home.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(home.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(home.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(home.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new home().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JPanel background;
    private javax.swing.JLabel bg;
    private javax.swing.JLabel convert;
    private javax.swing.JLabel fg;
    private javax.swing.JLabel filename;
    private javax.swing.JPanel foreground;
    private javax.swing.JLabel hall;
    private javax.swing.JLabel image;
    private javax.swing.JLabel loadergif;
    private javax.swing.JLabel opensoft;
    private javax.swing.JLabel pdf;
    private javax.swing.JFileChooser pickimage;
    private javax.swing.JFileChooser pickpdf;
    private javax.swing.JLabel report;
    private javax.swing.JLabel title;
    // End of variables declaration//GEN-END:variables
}
