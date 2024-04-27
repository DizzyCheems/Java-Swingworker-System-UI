import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.Vector;
import javax.swing.BorderFactory;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JOptionPane;
import javax.swing.SwingWorker;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.awt.Font;


public class NorthWindRead extends javax.swing.JFrame {

    ResultSet rs;
    Connection conn;
    Statement stm1;
    PreparedStatement pst1;

    private static String driver = "com.mysql.jdbc.Driver";
    private static String domain = "jdbc:mysql:///northwind";
    private static String user = "root";
    private static String pass = "";
    static int count = 0;
    double asd = 0;
    double sda = 0;
    double das = 0;
    double result;

    public NorthWindRead() {
        initComponents();
        connect();
        loadStats();
        tblOrderDetailslColumnRenderer();
        setTableBorderColor(Color.BLUE);
        setDarkMode();

        // Add ListSelectionListener to tblSales1
        tblSales1.getSelectionModel().addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent e) {
                if (!e.getValueIsAdjusting()) {
                    int selectedRow = tblSales1.getSelectedRow();
                    if (selectedRow != -1) {
                        String selectedOrderId = tblSales1.getValueAt(selectedRow, 0).toString();
                        updateTable2(selectedOrderId);
                    }
                }
            }
        });

        // Add ActionListener to JComboBox
        jComboBox1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String selectedContactName = (String) jComboBox1.getSelectedItem();
                if (selectedContactName != null) {
                    updateTable1(selectedContactName);
                    updateTextfields(selectedContactName);
                    updateJLabel1(selectedContactName); // Update JLabel1
                }
            }
        });

    }

    private void setTableBorderColor(Color color) {
        tblSales1.setBorder(BorderFactory.createLineBorder(color));
        tblSales2.setBorder(BorderFactory.createLineBorder(color));
    }

    private void setDarkMode() {
        Color darkBackground = new Color(34, 40, 49);
        Color grey = new Color(26, 26, 64);

        Color darkForeground = Color.WHITE;
        getContentPane().setBackground(darkBackground);

        jLabel1.setForeground(darkForeground);
        jLabel2.setForeground(darkForeground);
        jLabel3.setForeground(darkForeground);
        jLabel4.setForeground(darkForeground);
        jLabel5.setForeground(darkForeground);
        jLabel6.setForeground(darkForeground);
        jTextField2.setForeground(darkForeground);
        jTextField3.setForeground(darkForeground);
        jTextField4.setForeground(darkForeground);
        jTextField5.setForeground(darkForeground);
        jTextField6.setForeground(darkForeground);
        jTextField7.setForeground(darkForeground);
        jComboBox1.setForeground(darkForeground);
        tblSales1.setForeground(darkForeground);
        tblSales2.setForeground(darkForeground);

        jLabel1.setBackground(darkBackground);
        jLabel2.setBackground(darkBackground);
        jLabel3.setBackground(darkBackground);
        jLabel4.setBackground(darkBackground);
        jLabel5.setBackground(darkBackground);
        jLabel6.setBackground(darkBackground);
        jTextField2.setBackground(darkBackground);
        jTextField3.setBackground(darkBackground);
        jTextField4.setBackground(darkBackground);
        jTextField5.setBackground(darkBackground);
        jTextField6.setBackground(darkBackground);
        jTextField7.setBackground(darkBackground);
        jComboBox1.setBackground(darkBackground);

        tblSales1.setBackground(grey);
        tblSales2.setBackground(grey);

        tblSales1.setSelectionBackground(new Color(70, 138, 210 ));
        tblSales1.setSelectionForeground(darkBackground);
        tblSales2.setSelectionBackground(new Color(70, 138, 210 ));
        tblSales2.setSelectionForeground(darkBackground);
    }

    private void connect() {
        try {
            Class.forName(driver);
            conn = DriverManager.getConnection(domain, user, pass);
            JOptionPane.showMessageDialog(null, "Successfully connected to the MySQL Server");
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, "connection error");
        }
    }

 public void loadStats() {
    SwingWorker<Void, Vector<String>> worker = new SwingWorker<>() {
        @Override
        protected Void doInBackground() {
            count = 0;
            String strSQL1 = "SELECT * FROM salesorder";
            String strSQL2 = "SELECT contactName FROM customer ORDER BY contactName"; // Add ORDER BY clause
            try {
                stm1 = conn.createStatement();
                rs = stm1.executeQuery(strSQL1);
                while (rs.next()) {
                    ++count;
                    Vector<String> rowData = new Vector<>();
                    rowData.add(rs.getString(1));
                    rowData.add(rs.getString(2));
                    rowData.add(rs.getString(3));
                    rowData.add(rs.getString(4));
                    publish(rowData);
                }

                PreparedStatement pstmt = conn.prepareStatement(strSQL2);
                ResultSet resultSet = pstmt.executeQuery();
                while (resultSet.next()) {
                    String contactName = resultSet.getString("contactName");
                    Vector<String> comboData = new Vector<>();
                    comboData.add(contactName);
                    publish(comboData);
                }

            } catch (Exception e) {
                JOptionPane.showMessageDialog(null, e.toString());
            }

            jTextField5.setText(formatWithCommas(count) + " elements");

            return null;
        }

        @Override
        protected void process(java.util.List<Vector<String>> chunks) {
            DefaultTableModel tableModel = (DefaultTableModel) tblSales1.getModel();
            for (Vector<String> rowData : chunks) {
                if (rowData.size() > 1) {
                    tableModel.addRow(rowData);
                } else {
                    for (String contactName : rowData) {
                        jComboBox1.addItem(contactName);
                    }
                }
            }
        }

        @Override
        protected void done() {
            // Optional: perform any additional UI updates or logic after the background task is done
        }
    };
    worker.execute();
}


private int updateTable1(String selectedContactName) {
    DefaultTableModel tableModel = (DefaultTableModel) tblSales1.getModel();
    tableModel.setRowCount(0); 
    int rowCount = 0;
    String strSQL = "SELECT * FROM salesorder WHERE custId IN (SELECT custId FROM customer WHERE contactName = ?)";
    try {
        pst1 = conn.prepareStatement(strSQL);
        pst1.setString(1, selectedContactName);
        ResultSet resultSet = pst1.executeQuery();
        while (resultSet.next()) {
            Vector<String> rowData = new Vector<>();
            rowData.add(resultSet.getString(1)); 
           
            String orderDate = resultSet.getString(4);
            String formattedDate = formatDate(orderDate); 
            rowData.add(formattedDate); 
            rowData.add(resultSet.getString(14)); 
            rowData.add(resultSet.getString(11)); 
            tableModel.addRow(rowData);
            rowCount++; // Increment the row count
        }
    } catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.toString());
    }
    return rowCount; 
}


private String formatDate(String date) {
    
    try {
        SimpleDateFormat sdfInput = new SimpleDateFormat("yyyy-MM-dd"); 
        Date parsedDate = sdfInput.parse(date);
        SimpleDateFormat sdfOutput = new SimpleDateFormat("dd/MM/yy");
        return sdfOutput.format(parsedDate);
    } catch (ParseException e) {
        e.printStackTrace();
        return date; 
    }
}



private void updateJLabel1(String selectedContactName) {
    int rowCount = updateTable1(selectedContactName);
    jLabel1.setText("There is " + rowCount + " order record for " + selectedContactName);
}



private void updateTextfields(String selectedContactName) {
    String strSQL = "SELECT custId, companyName, country FROM customer WHERE contactName = ?";
    try {
        pst1 = conn.prepareStatement(strSQL);
        pst1.setString(1, selectedContactName);
        ResultSet resultSet = pst1.executeQuery();
        if (resultSet.next()) {
            // Update text fields based on the retrieved data
            jTextField2.setText(resultSet.getString("custId"));
            jTextField3.setText(resultSet.getString("companyName"));
            jTextField4.setText(resultSet.getString("country"));
        }
    } catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.toString());
    }
}


    private String formatWithCommas(double value) {
        // Your existing formatWithCommas method implementation
    }

    private boolean updatingTable2 = false;
private void updateTable2(String selectedOrderId) {
    DefaultTableModel tableModel = (DefaultTableModel) tblSales2.getModel();
    tableModel.setRowCount(0); 
    double sumAmount = 0.0; 
    double sumDiscount = 0.0; 
    double totalmarkdown = 0.0; 
    int orderDetailCount = 0;
    String strSQL = "SELECT od.productId, " +
            "ROUND(p.unitPrice, 2), " +
            "od.quantity, " +
            "ROUND(od.discount, 2), " +
            "FORMAT(p.unitPrice * od.quantity, 2) AS Amount, " +
            "FORMAT(od.discount * (p.unitPrice * od.quantity), 2) AS Discount, " +
            "FORMAT((1 - od.discount) * (p.unitPrice * od.quantity), 2) AS DiscountedValue " +
            "FROM orderdetail od " +
            "JOIN product p ON od.productId = p.productId " +
            "WHERE orderId = ?";
    try {
        pst1 = conn.prepareStatement(strSQL);
        pst1.setString(1, selectedOrderId);
        ResultSet resultSet = pst1.executeQuery();
        DecimalFormat df = new DecimalFormat("#,##0.00");
        while (resultSet.next()) {
            Vector<String> rowData = new Vector<>();
            rowData.add(resultSet.getString(1)); 
            rowData.add(df.format(Double.parseDouble(resultSet.getString(2)))); 
            rowData.add(resultSet.getString(3)); 
            rowData.add(resultSet.getString(4)); 
            rowData.add(df.format(Double.parseDouble(resultSet.getString(5).replace(",", "")))); // Amount
            rowData.add(df.format(Double.parseDouble(resultSet.getString(6).replace(",", "")))); // Discount
            rowData.add(df.format(Double.parseDouble(resultSet.getString(7).replace(",", "")))); // Discounted Value
            tableModel.addRow(rowData);

         
            orderDetailCount++;

            sumAmount += Double.parseDouble(resultSet.getString(5).replace(",", ""));

        
            sumDiscount += Double.parseDouble(resultSet.getString(6).replace(",", ""));

    
            totalmarkdown += Double.parseDouble(resultSet.getString(7).replace(",", ""));
        }




        for (int i = 0; i < tblSales2.getColumnCount(); i++) {
            tblSales2.getColumnModel().getColumn(i).setPreferredWidth(150); 
        }
    } catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.toString());
    }
}


    private String getProductName(String productId) {
        String productName = "";
        String strSQL = "SELECT productName FROM product WHERE productId = ?";
        try {
            pst1 = conn.prepareStatement(strSQL);
            pst1.setString(1, productId);
            ResultSet resultSet = pst1.executeQuery();
            if (resultSet.next()) {
                productName = resultSet.getString(1);
            }
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, e.toString());
        }
        return productName;
    }

    private void tblOrderDetailslColumnRenderer() {
        // Your existing tblOrderDetailslColumnRenderer method implementation
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane1 = new javax.swing.JScrollPane();
        tblSales2 = new javax.swing.JTable();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        jLabel5 = new javax.swing.JLabel();
        jTextField2 = new javax.swing.JTextField();
        jTextField3 = new javax.swing.JTextField();
        jTextField4 = new javax.swing.JTextField();
        jScrollPane2 = new javax.swing.JScrollPane();
        tblSales1 = new javax.swing.JTable();
        jComboBox1 = new javax.swing.JComboBox<>();
        jLabel1 = new javax.swing.JLabel();
        jLabel6 = new javax.swing.JLabel();
        jTextField5 = new javax.swing.JTextField();
        jTextField6 = new javax.swing.JTextField();
        jTextField7 = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);


   

        jLabel2.setText("Customer ID");

        jLabel3.setText("Company Name");

        jLabel4.setText("Country");

        jLabel5.setText("Contact name");

        jTextField2.setEditable(false);

        jTextField3.setEditable(false);

        jTextField4.setEditable(false);




javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
getContentPane().setLayout(layout);
layout.setHorizontalGroup(
    layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
        .addGroup(layout.createSequentialGroup()
            .addContainerGap()
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                .addComponent(jScrollPane2)
                .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jLabel2)
                        .addComponent(jLabel3)
                        .addComponent(jLabel4)
                        .addComponent(jLabel5))
                    .addGap(63, 63, 63)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jTextField4, javax.swing.GroupLayout.PREFERRED_SIZE, 130, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jTextField3, javax.swing.GroupLayout.PREFERRED_SIZE, 130, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jTextField2, javax.swing.GroupLayout.PREFERRED_SIZE, 130, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jComboBox1, javax.swing.GroupLayout.PREFERRED_SIZE, 130, javax.swing.GroupLayout.PREFERRED_SIZE)))) // Set preferred size for jComboBox1
            .addGap(18, 18, 18)
            .addContainerGap())
);
layout.setVerticalGroup(
    layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
        .addGroup(layout.createSequentialGroup()
            .addContainerGap()
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabel2)
                        .addComponent(jTextField2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGap(18, 18, 18)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabel3)
                        .addComponent(jTextField3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGap(18, 18, 18)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabel4)
                        .addComponent(jTextField4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGap(18, 18, 18)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabel5)
                        .addComponent(jComboBox1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGap(18, 18, 18)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 275, javax.swing.GroupLayout.PREFERRED_SIZE)))
            .addContainerGap(37, Short.MAX_VALUE))
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
            java.util.logging.Logger.getLogger(NorthWindRead.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(NorthWindRead.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(NorthWindRead.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(NorthWindRead.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        //</editor-fold>
        //</editor-fold>
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new NorthWindRead().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JComboBox<String> jComboBox1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTextField jTextField2;
    private javax.swing.JTextField jTextField3;
    private javax.swing.JTextField jTextField4;
    private javax.swing.JTextField jTextField5;
    private javax.swing.JTextField jTextField6;
    private javax.swing.JTextField jTextField7;
    private javax.swing.JTable tblSales1;
    private javax.swing.JTable tblSales2;
    // End of variables declaration//GEN-END:variables
}
