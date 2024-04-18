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
import javax.swing.table.TableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.awt.Font;
import java.awt.Component;
import javax.swing.JTable;
import javax.swing.JButton; 
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.ListSelectionModel;
import javax.swing.JFrame;
import java.awt.GridLayout;
import javax.swing.JTextField;
import javax.swing.JComboBox;
import java.util.HashMap;
import java.awt.Dimension;
import javax.swing.Box; 
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.sql.SQLException;

//<!--BUILT BY Justine Favia-->
public class NorthwindProjectSystem extends javax.swing.JFrame {

    ResultSet rs;
    Connection conn;
    Statement stm1;
    PreparedStatement pst1;

    private static String driver = "com.mysql.jdbc.Driver";
    private static String domain = "jdbc:mysql:///northwind";
    private static String user = "root";
    private static String pass = "";
    private String selectedOrderId;
    private String orderId;
    static int count = 0;
    double asd = 0;
    double sda = 0;
    double das = 0;
    double result;

    public NorthwindProjectSystem() {
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

                    }
                }
            }
        });


// Assuming this is inside a class that extends JFrame or JPanel
tblSales2.addMouseListener(new MouseAdapter() {
    @Override
    public void mouseClicked(MouseEvent e) {
        int row = tblSales2.rowAtPoint(e.getPoint());
        int col = tblSales2.columnAtPoint(e.getPoint());
        if (row >= 0 && col >= 0) {
            Object value = tblSales2.getValueAt(row, col);
            if (value != null && value.toString().equals("DELETE")) {
                String orderDetailId = tblSales2.getValueAt(row, 0).toString(); // Assuming the ID is in the first column
                int confirmDialog = JOptionPane.showConfirmDialog(null, "Are you sure you want to delete OrderDetail ID: " + orderDetailId + "?", "Confirm Deletion", JOptionPane.YES_NO_OPTION);
                if (confirmDialog == JOptionPane.YES_OPTION) {
                    deleteOrderDetail(orderDetailId);
                }
            }
        }
    }
});

        tblSales2.getColumnModel().getColumn(7).setCellRenderer(new ButtonCellRenderer());
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
        jLabel8.setForeground(darkForeground);
        jTextField2.setForeground(darkForeground);
        jTextField3.setForeground(darkForeground);
        jTextField4.setForeground(darkForeground);
        jTextField5.setForeground(darkForeground);
        jTextField6.setForeground(darkForeground);
        jTextField7.setForeground(darkForeground);
        jTextField8.setForeground(darkForeground);
        jComboBox1.setForeground(darkForeground);
        tblSales1.setForeground(darkForeground);
        tblSales2.setForeground(darkForeground);

        jLabel1.setBackground(darkBackground);
        jLabel2.setBackground(darkBackground);
        jLabel3.setBackground(darkBackground);
        jLabel4.setBackground(darkBackground);
        jLabel5.setBackground(darkBackground);
        jLabel6.setBackground(darkBackground);
        jLabel8.setBackground(darkBackground);

        jTextField2.setBackground(darkBackground);
        jTextField3.setBackground(darkBackground);
        jTextField4.setBackground(darkBackground);
        jTextField5.setBackground(darkBackground);
        jTextField6.setBackground(darkBackground);
        jTextField7.setBackground(darkBackground);
        jTextField8.setBackground(darkBackground);
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
        // Set the selection mode of tblSales1 to single selection
    tblSales1.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

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


private void handleRowClick(int rowIndex) {
    // Check if the rowIndex is valid
    if (rowIndex >= 0 && rowIndex < tblSales1.getRowCount()) {
        // Get the data from the selected row
        String orderId = tblSales1.getValueAt(rowIndex, 0).toString(); // Assuming orderId is in the first column
        String orderDate = tblSales1.getValueAt(rowIndex, 1).toString(); // Assuming orderDate is in the second column
        // Perform any action with the selected data
        System.out.println("Clicked row: " + orderId + ", " + orderDate);
    }
}

private int updateTable1(String selectedContactName) {
    DefaultTableModel tableModel = (DefaultTableModel) tblSales1.getModel();
    tableModel.setRowCount(0); 
    int rowCount = 0;
    String strSQL = "SELECT * FROM salesorder WHERE custId IN (SELECT custId FROM customer WHERE contactName = ?) ORDER BY orderDate DESC"; // Assuming orderDate is the column that determines the order
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
                 
            String requiredDate = resultSet.getString(5);
            if (requiredDate != null) {
                String formattedRequiredDate = formatDate(requiredDate); 
                rowData.add(formattedRequiredDate); 
            } else {
                rowData.add(""); 
            }
             
             
                String shippingdate = resultSet.getString(6);
            if (shippingdate != null) {
                String formattedshippingDate = formatDate(shippingdate); 
                rowData.add(formattedshippingDate); 
            } else {
                rowData.add(""); 
            }
            rowData.add(resultSet.getString(11)); 
            
            tableModel.addRow(rowData);
            rowCount++; // Increment the row count
        }

        // Clear the displayed data in jTextField5, jTextField6, jTextField7, and tblSales2
        clearDisplayedData();

        // Automatically click the first row element
        if (tblSales1.getRowCount() > 0) {
            tblSales1.setRowSelectionInterval(0, 0); // Select the first row
            handleRowClick(tblSales1.getSelectedRow());
        }

    } catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.toString());
    }
    return rowCount; 
}


private void clearDisplayedData() {
    jTextField5.setText("");
    jTextField6.setText("");
    jTextField7.setText("");
    jTextField8.setText("");
    jLabel6.setText("...");
    DefaultTableModel tableModel = (DefaultTableModel) tblSales2.getModel();
    tableModel.setRowCount(0);
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
    String strSQL = "SELECT orderId, orderDate, shippedDate, requiredDate FROM salesorder INNER JOIN customer ON salesorder.custId = customer.custId WHERE customer.contactName = ? ORDER BY orderId DESC";

    try {
        pst1 = conn.prepareStatement(strSQL);
        pst1.setString(1, selectedContactName);
        ResultSet resultSet = pst1.executeQuery();
        if (resultSet.next()) {
            // Format date using SimpleDateFormat
            SimpleDateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy"); // Adjust the format based on your database date format

            // Update text fields based on the retrieved data
            jTextField2.setText(resultSet.getString("orderId"));
            jTextField3.setText(dateFormat.format(resultSet.getTimestamp("orderDate")));
            jTextField4.setText(dateFormat.format(resultSet.getTimestamp("requiredDate")));
            jTextField8.setText(dateFormat.format(resultSet.getTimestamp("orderDate"))); // Set the shipping date in JTextField8
        } else {
            // Clear text fields if no data is found
            jTextField2.setText("");
            jTextField3.setText("");
            jTextField4.setText("");
            jTextField8.setText("");
        }
    } catch (Exception e) {
        JOptionPane.showMessageDialog(null, e.toString());
    }
}




    private String formatWithCommas(double value) {
        // Your existing formatWithCommas method implementation
    }

private void deleteOrderDetail(String orderDetailId) {
    String deleteSQL = "DELETE FROM orderdetail WHERE orderDetailId = ?";
    try {
        PreparedStatement pstDelete = conn.prepareStatement(deleteSQL);
        pstDelete.setString(1, orderDetailId);
        int rowsAffected = pstDelete.executeUpdate();
        if (rowsAffected > 0) {
            // Refresh the table after deletion
           
            // Check if tblSales2 has rows before trying to select the first row
            if (tblSales2.getRowCount() > 0) {
                // Find the row index of the deleted order detail
                int rowIndex = findRowIndex(tblSales2, orderDetailId);
                if (rowIndex != -1) {
                    DefaultTableModel model = (DefaultTableModel) tblSales2.getModel();
                    model.removeRow(rowIndex); // Remove the row from tblSales2
                }
            }

            // Automatically click the first row element in tblSales1
            if (tblSales1.getRowCount() > 0) {
                tblSales1.setRowSelectionInterval(0, 0); // Select the first row in tblSales1
                handleRowClick(tblSales1.getSelectedRow());
            } else {
                // Handle the case when tblSales1 is empty after deletion
                System.out.println("tblSales1 is empty after deletion.");
            }
        } else {
            JOptionPane.showMessageDialog(null, "Failed to delete OrderDetail ID: " + orderDetailId);
        }
        pstDelete.close();
    } catch (SQLException e) {
        JOptionPane.showMessageDialog(null, "Error deleting OrderDetail: " + e.getMessage());
    }
}

private int findRowIndex(JTable table, String orderDetailId) {
    for (int i = 0; i < table.getRowCount(); i++) {
        if (table.getValueAt(i, 0).toString().equals(orderDetailId)) {
            return i; // Found the row index
        }
    }
    return -1; // Row not found
}

private void updateTable2(String selectedOrderId) {
    DefaultTableModel tableModel = (DefaultTableModel) tblSales2.getModel();
    tableModel.setRowCount(0); 
    double sumAmount = 0.0; 
    double sumDiscount = 0.0; 
    double totalMarkdown = 0.0; 
    int orderDetailCount = 0;
    String strSQL = "SELECT od.orderDetailId, " +
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

        // Set the custom cell renderer for the "DELETE" column
        tblSales2.getColumnModel().getColumn(7).setCellRenderer(new ButtonCellRenderer());

        while (resultSet.next()) {
            Vector<String> rowData = new Vector<>();
            rowData.add(resultSet.getString(1)); // orderDetailId
            rowData.add(df.format(Double.parseDouble(resultSet.getString(2)))); // unitPrice
            rowData.add(resultSet.getString(3)); // quantity
            rowData.add(resultSet.getString(4)); // discount
            rowData.add(df.format(Double.parseDouble(resultSet.getString(5).replace(",", "")))); // Amount
            rowData.add(df.format(Double.parseDouble(resultSet.getString(6).replace(",", "")))); // Discount
            rowData.add(df.format(Double.parseDouble(resultSet.getString(7).replace(",", "")))); // Discounted Value
            rowData.add("DELETE"); 
            tableModel.addRow(rowData);

            orderDetailCount++;
            sumAmount += Double.parseDouble(resultSet.getString(5).replace(",", ""));
            sumDiscount += Double.parseDouble(resultSet.getString(6).replace(",", ""));
            totalMarkdown += Double.parseDouble(resultSet.getString(7).replace(",", ""));
        }

        jTextField5.setText(df.format(sumAmount));
        jTextField6.setText(df.format(sumDiscount));
        jTextField7.setText(df.format(totalMarkdown));
        jLabel6.setText("There are " + orderDetailCount + " product order records for orderId " + selectedOrderId);

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
        jLabel8 = new javax.swing.JLabel();
        jTextField2 = new javax.swing.JTextField();
        jTextField3 = new javax.swing.JTextField();
        jTextField4 = new javax.swing.JTextField();
        jTextField8 = new javax.swing.JTextField();
        jScrollPane2 = new javax.swing.JScrollPane();
        tblSales1 = new javax.swing.JTable();
        jComboBox1 = new javax.swing.JComboBox<>();
        jLabel1 = new javax.swing.JLabel();
        jLabel6 = new javax.swing.JLabel();
        jTextField5 = new javax.swing.JTextField();
        jTextField6 = new javax.swing.JTextField();
        jTextField7 = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        tblSales2.setModel(new javax.swing.table.DefaultTableModel(
                new Object [][] {

                },
                new String [] {
                        "Order Detail ID", "Unit Price", "Quantity", "% Discount", "Amount", "Discount", "Discounted Value", "Action"
                }
        ));
        jScrollPane1.getViewport().setBackground(new Color(33, 36, 106));
        jScrollPane1.setViewportView(tblSales2);
        jLabel2.setText("Order ID");

        jLabel3.setText("Order Date");

        jLabel4.setText("Date Required");

        jLabel5.setText("Contact name");

        jLabel8.setText("Shipping Date");

        jTextField2.setEditable(false);

        jTextField3.setEditable(false);

        jTextField4.setEditable(false);

        tblSales1.setModel(new javax.swing.table.DefaultTableModel(
                new Object [][] {

                },
                new String [] {
                        "OrderID", "Order Date", "Required Date", "Shipping Date"
                }
        ));
         jScrollPane2.getViewport().setBackground(new Color(33, 36, 106));
         jScrollPane2.setViewportView(tblSales1);

        jLabel1.setFont(new Font("Arial", Font.PLAIN, 14));
        jLabel6.setFont(new Font("Arial", Font.PLAIN, 14));


    jTextField5.setFont(jTextField5.getFont().deriveFont(16f));
    jTextField6.setFont(jTextField6.getFont().deriveFont(16f));
    jTextField7.setFont(jTextField7.getFont().deriveFont(16f));


JButton btnAddProduct = new JButton("Add Product");
btnAddProduct.addActionListener(new ActionListener() {
    public void actionPerformed(ActionEvent e) {
        // Create an instance of your new JPanel with GridBagLayout
        JPanel addProductPanel = new JPanel(new GridBagLayout());

        // Create GridBagConstraints for label alignment on the left
        GridBagConstraints gbcLeft = new GridBagConstraints();
        gbcLeft.anchor = GridBagConstraints.WEST;
        gbcLeft.gridx = 0;
        gbcLeft.gridy = GridBagConstraints.RELATIVE; // Start at the current row and move down
        gbcLeft.insets = new Insets(5, 5, 5, 5); // Add insets for spacing

        // Create GridBagConstraints for text field alignment on the right
        GridBagConstraints gbcRight = new GridBagConstraints();
        gbcRight.anchor = GridBagConstraints.WEST;
        gbcRight.gridx = 1;
        gbcRight.gridy = GridBagConstraints.RELATIVE; // Start at the current row and move down
        gbcRight.weightx = 1.0; // Allow text fields to expand horizontally
        gbcRight.insets = new Insets(5, 5, 5, 5); // Add insets for spacing

        // Add JLabels for the fields
        JLabel lblProduct = new JLabel("Product");
        JLabel lblQuantity = new JLabel("Quantity");
        JLabel lblDiscount = new JLabel("Discount");
        JLabel lblUnitPrice = new JLabel("Unit Price");

        // Set font size for JLabels
        Font labelFont = lblProduct.getFont();
        lblProduct.setFont(new Font(labelFont.getName(), Font.PLAIN, 20)); // Example size: 20
        lblQuantity.setFont(new Font(labelFont.getName(), Font.PLAIN, 20)); // Example size: 20
        lblDiscount.setFont(new Font(labelFont.getName(), Font.PLAIN, 20)); // Example size: 20
        lblUnitPrice.setFont(new Font(labelFont.getName(), Font.PLAIN, 20)); // Example size: 20

        // Fetch product names from the database
        JComboBox<String> cmbProductName = new JComboBox<>();
        HashMap<String, Double> productPrices = new HashMap<>(); // Use HashMap instead of Map

        try {
            Class.forName("com.mysql.cj.jdbc.Driver"); // Use the new MySQL driver class
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/northwind", "root", "");
            Statement stmt = conn.createStatement();
            ResultSet rs = stmt.executeQuery("SELECT productName, unitPrice FROM product ORDER BY productName");
            while (rs.next()) {
                String productName = rs.getString("productName");
                double unitPrice = rs.getDouble("unitPrice");
                cmbProductName.addItem(productName);
                productPrices.put(productName, unitPrice); // Store product prices in the map
            }
            rs.close();
            stmt.close();
            conn.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        // Add JTextFields for input
        JTextField txtQuantity = new JTextField();
        JTextField txtDiscount = new JTextField();
        JTextField txtUnitPrice = new JTextField();
        // Set font size for JTextFields
        Font textFieldFont = txtQuantity.getFont();
        txtQuantity.setFont(new Font(textFieldFont.getName(), Font.PLAIN, 20)); // Example size: 20
        txtDiscount.setFont(new Font(textFieldFont.getName(), Font.PLAIN, 20)); // Example size: 20
        txtUnitPrice.setFont(new Font(textFieldFont.getName(), Font.PLAIN, 20)); // Example size: 20
        // Set preferred size for text fields (adjust as needed)
        txtQuantity.setPreferredSize(new Dimension(200, 40)); // Adjust height as needed
        txtDiscount.setPreferredSize(new Dimension(200, 40)); // Adjust height as needed
        txtUnitPrice.setPreferredSize(new Dimension(200, 40)); // Adjust height as needed

        // Add ActionListener to update unit price when product is selected
        cmbProductName.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String selectedProductName = cmbProductName.getSelectedItem().toString();
                if (productPrices.containsKey(selectedProductName)) {
                    double unitPrice = productPrices.get(selectedProductName);
                    txtUnitPrice.setText(String.valueOf(unitPrice)); // Update the unit price field
                }
            }
        });



        // Add components to the panel with GridBagConstraints
        addProductPanel.add(lblProduct, gbcLeft);
        addProductPanel.add(cmbProductName, gbcRight);
        addProductPanel.add(lblQuantity, gbcLeft);
        addProductPanel.add(txtQuantity, gbcRight);
        addProductPanel.add(lblDiscount, gbcLeft);
        addProductPanel.add(txtDiscount, gbcRight);
        addProductPanel.add(lblUnitPrice, gbcLeft);
        addProductPanel.add(txtUnitPrice, gbcRight);

        // Add an empty component for spacing between rows
        GridBagConstraints gbcSpace = new GridBagConstraints();
        gbcSpace.gridx = 0;
        gbcSpace.gridy = GridBagConstraints.RELATIVE; // Start at the current row and move down
        gbcSpace.gridwidth = 2; // Span across both columns
        gbcSpace.weighty = 1.0; // Allow empty component to expand vertically
        addProductPanel.add(Box.createVerticalStrut(0), gbcSpace); // Adjust height as needed

        // Set preferred size for the panel
        addProductPanel.setPreferredSize(new Dimension(400, 400)); // Adjust size as needed

        // Create a JFrame to hold the panel
        JFrame frame = new JFrame("Add Product");
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        frame.getContentPane().add(addProductPanel);
        frame.pack(); // Pack components within the frames
        frame.setVisible(true);

 JButton btnAdd = new JButton("Add");
btnAdd.addActionListener(new ActionListener() {
    @Override
    public void actionPerformed(ActionEvent e) {    
        // Get the selected order ID from tblSales1
        int selectedRow = tblSales1.getSelectedRow();
        if (selectedRow != -1) {
            String orderId = tblSales1.getValueAt(selectedRow, 0).toString();

            // Get the selected product name and retrieve its ID
            String selectedProductName = cmbProductName.getSelectedItem().toString();
            int productId = getProductId(selectedProductName);

            // Get the quantity, discount, and unit price values
            int quantity = Integer.parseInt(txtQuantity.getText());
            double discountInput = Double.parseDouble(txtDiscount.getText());
            double discount = Math.floor(discountInput) / 100.0; // Round down and convert to percentage (e.g., 3 becomes 0.03)
            double unitPrice = Double.parseDouble(txtUnitPrice.getText());

            // Insert the data into the orderdetail table
            insertOrderDetail(orderId, productId, quantity, discount, unitPrice);
            updateTable2(orderId);
        }
        frame.setVisible(false); // Hide the JFrame
    }
});

// Set preferred size for the "Add" button
btnAdd.setPreferredSize(new Dimension(100, 40)); // Adjust size as needed

// Add ActionListener for the "Add" button in the JPanel
addProductPanel.add(btnAdd, gbcLeft); // Add the "Add" button to the JPanel with left alignment
 // Add the "Add" button to the JPanel with right alignment
    }
    // Method to retrieve the product ID based on the product name
    private int getProductId(String productName) {
        int productId = -1; // Default value if product ID is not found
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/northwind", "root", "");
            PreparedStatement pstmt = conn.prepareStatement("SELECT productId FROM product WHERE productName = ?");
            pstmt.setString(1, productName);
            ResultSet rs = pstmt.executeQuery();
            if (rs.next()) {
                productId = rs.getInt("productId");
            }
            rs.close();
            pstmt.close();
            conn.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return productId;
    }

    // Method to insert data into the orderdetail table
    private void insertOrderDetail(String orderId, int productId, int quantity, double discount, double unitPrice) {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/northwind", "root", "");
            PreparedStatement pstmt = conn.prepareStatement("INSERT INTO orderdetail (orderId, productId, quantity, discount, unitPrice) VALUES (?, ?, ?, ?, ?)");
            pstmt.setString(1, orderId);
            pstmt.setInt(2, productId);
            pstmt.setInt(3, quantity);
            pstmt.setDouble(4, discount);
            pstmt.setDouble(5, unitPrice);
            pstmt.executeUpdate();
            pstmt.close();
            conn.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
});


JButton btnAddSalesOrder = new JButton("Add Sales Order");
btnAddSalesOrder.addActionListener(new ActionListener() {
    public void actionPerformed(ActionEvent e) {
        try {

            // Load the JDBC driver and establish a connection
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/northwind", "root", "");

            // Get the selected contactName from the JComboBox
            String selectedContactName = (String) jComboBox1.getSelectedItem();

            // Prepare the SQL statement to get the custId based on the selected contactName
            PreparedStatement pstmtCustId = conn.prepareStatement("SELECT custId FROM customer WHERE contactName = ?");
            pstmtCustId.setString(1, selectedContactName);
            ResultSet rsCustId = pstmtCustId.executeQuery();

            // Check if a custId is retrieved
            if (rsCustId.next()) {
                int custId = rsCustId.getInt("custId");

                // Prepare the SQL statement to insert a new sales order
                PreparedStatement pstmt = conn.prepareStatement("INSERT INTO salesorder (custId, orderDate, requiredDate, shippedDate, shipperId, employeeId) VALUES (?, NOW(), DATE_ADD(NOW(), INTERVAL 3 DAY), DATE_ADD(NOW(), INTERVAL 5 DAY), 1, 1)");

                pstmt.setInt(1, custId);

                pstmt.executeUpdate();

                pstmt.close();
                conn.close();

                JOptionPane.showMessageDialog(null, "Sales order added successfully.");
                updateTable1(selectedContactName); // Call the updateTable1 method to refresh the table        
            } else {
                JOptionPane.showMessageDialog(null, "Error: Customer ID not found for the selected contact name.", "Error", JOptionPane.ERROR_MESSAGE);
            }

            rsCustId.close();
            pstmtCustId.close();
        } catch (Exception ex) {
            ex.printStackTrace();
            JOptionPane.showMessageDialog(null, "Error adding sales order: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }
});




        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(layout.createSequentialGroup()
                                .addContainerGap()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                                        .addComponent(jLabel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                        .addComponent(jScrollPane2)
                                        .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                        .addComponent(jLabel2)
                                                        .addComponent(jLabel3)
                                                        .addComponent(jLabel4)
                                                        .addComponent(btnAddSalesOrder)
                                                                
                                                        .addComponent(jLabel5))
                                                .addGap(63, 63, 63)
                                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                        .addComponent(jTextField4, javax.swing.GroupLayout.PREFERRED_SIZE, 130, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                        .addComponent(jTextField3, javax.swing.GroupLayout.PREFERRED_SIZE, 130, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                        .addComponent(jTextField2, javax.swing.GroupLayout.PREFERRED_SIZE, 130, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                        .addComponent(jComboBox1, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
                                .addGap(18, 18, 18)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addComponent(jLabel6, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                        .addGroup(layout.createSequentialGroup()
                                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                                .addComponent(jTextField5, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 130, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                                .addComponent(jTextField6, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 130, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                                .addComponent(jTextField7, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 130, javax.swing.GroupLayout.PREFERRED_SIZE))
                                                       
                                                         .addComponent(btnAddProduct)
                                                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 800, javax.swing.GroupLayout.PREFERRED_SIZE))
                                                .addGap(0, 0, Short.MAX_VALUE)))
                                .addContainerGap())
        );
        layout.setVerticalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(layout.createSequentialGroup()
                                .addContainerGap()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addGroup(layout.createSequentialGroup()
                                                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 275, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addComponent(jTextField5, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addComponent(jTextField6, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addComponent(jTextField7, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                  .addComponent(btnAddProduct)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addComponent(jLabel6))
                                           
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
                                                        .addComponent(btnAddSalesOrder)
                                                   
                                                .addGap(18, 18, 18)
                                                .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 275, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jLabel1)
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
            java.util.logging.Logger.getLogger(NorthwindProjectSystem.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(NorthwindProjectSystem.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(NorthwindProjectSystem.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(NorthwindProjectSystem.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new NorthwindProjectSystem().setVisible(true);
            }
        });
    }


   static class ButtonCellRenderer extends DefaultTableCellRenderer {
        private final JButton button;

        public ButtonCellRenderer() {
            button = new JButton("DELETE");
            button.setFocusPainted(false);
        }

        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
            return button;
        }
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JComboBox<String> jComboBox1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel8;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTextField jTextField2;
    private javax.swing.JTextField jTextField3;
    private javax.swing.JTextField jTextField4;
    private javax.swing.JTextField jTextField5;
    private javax.swing.JTextField jTextField6;
    private javax.swing.JTextField jTextField7;
    private javax.swing.JTextField jTextField8;
    private javax.swing.JTable tblSales1;
    private javax.swing.JTable tblSales2;
    // End of variables declaration//GEN-END:variables
}