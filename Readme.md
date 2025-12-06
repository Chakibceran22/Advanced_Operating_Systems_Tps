# 📦 Inventory Management System - Architecture & Design

A simple Java inventory management system implementing **MVC architecture** and **design patterns**.

---

## 📋 Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Design Patterns](#design-patterns)
- [Project Structure](#project-structure)
- [Entity Relationships](#entity-relationships)
- [Component Responsibilities](#component-responsibilities)

---

## 🎯 Overview

This is a straightforward inventory management system that helps track products and stock levels. The system uses:
- **MVC Architecture** for clean separation
- **5 Design Patterns** (Singleton, Observer, DAO, Factory, Strategy)
- **MySQL Database** for data storage
- **Java Swing** for the user interface

### Core Features
- Manage products (add, edit, delete, view)
- Track stock levels across warehouses
- Get automatic alerts when stock is low
- View stock movement history
- User authentication with roles

---

## 🏗️ System Architecture

### High-Level Architecture

```
┌─────────────────────────────────────┐
│          VIEW (UI)                  │
│   - ProductView                     │
│   - StockView                       │
│   - DashboardView                   │
└──────────────┬──────────────────────┘
               │
               │ User Actions
               ▼
┌─────────────────────────────────────┐
│       CONTROLLER                    │
│   - ProductController               │
│   - StockController                 │
└──────────────┬──────────────────────┘
               │
               │ Business Logic
               ▼
┌─────────────────────────────────────┐
│         MODEL + DAO                 │
│   - Product, Stock entities         │
│   - ProductDAO, StockDAO            │
└──────────────┬──────────────────────┘
               │
               │ Singleton Pattern
               ▼
┌─────────────────────────────────────┐
│    DatabaseConnection               │
└──────────────┬──────────────────────┘
               │
               ▼
         ┌──────────┐
         │ Database │
         │  MySQL   │
         └──────────┘
```

### Data Flow Example
```
User clicks "Add Product" 
    → ProductView 
    → ProductController.addProduct() 
    → ProductDAO.save() 
    → DatabaseConnection.getInstance() 
    → MySQL Database
    → Return success
    → Update ProductView
```

---

## 🎨 Design Patterns

### 1. **Singleton Pattern** 🔐

**Where**: Database Connection  
**Why**: Only one database connection needed for the entire app

```
DatabaseConnection
├─ Private constructor (can't create multiple instances)
├─ Static getInstance() method
└─ Single connection shared by all DAOs
```

**Used by**: All DAO classes to get database connection

---

### 2. **Observer Pattern** 🔔

**Where**: Stock monitoring  
**Why**: Automatically alert when stock gets low/empty

```
Stock (Subject)
├─ Has list of observers
├─ When quantity changes → check thresholds
└─ If low → notify all observers

Observers:
├─ EmailNotifier → sends email
├─ DashboardNotifier → shows alert on screen
└─ LogNotifier → writes to log file
```

**Triggers**:
- Stock quantity ≤ minimum threshold → "LOW STOCK" alert
- Stock quantity = 0 → "OUT OF STOCK" alert

---

### 3. **Repository/DAO Pattern** 💾

**Where**: All database operations  
**Why**: Separate database code from business logic

```
ProductDAO (interface)
├─ add(product)
├─ getById(id)
├─ getAll()
├─ update(product)
└─ delete(id)

ProductDAOImpl (implementation)
└─ Contains all SQL queries
```

**One DAO for each entity**:
- ProductDAO
- StockDAO
- SupplierDAO
- CategoryDAO
- UserDAO

---

### 4. **Factory Pattern** 🏭

**Where**: Product creation  
**Why**: Create different types of products without specifying exact classes

```
ProductFactory
└─ createProduct(type) method

Product Types:
├─ SimpleProduct → regular items (pens, books)
├─ PerishableProduct → food items (needs expiry date)
└─ SerializedProduct → electronics (needs serial number)
```

**When to use**:
- User adds a product → Factory decides which type to create
- Makes it easy to add new product types later

**Example**:
```
// Instead of: new PerishableProduct(...)
// You do: ProductFactory.createProduct("PERISHABLE", ...)
```

---

### 5. **Strategy Pattern** 💰

**Where**: Pricing calculation  
**Why**: Different pricing rules for different customer types

```
PricingStrategy (interface)
└─ calculatePrice(basePrice, quantity)

Pricing Strategies:
├─ RetailPricing → full price
├─ WholesalePricing → bulk discount (10% off for 50+ items)
└─ VIPPricing → member discount (15% off)
```

**When to use**:
- Calculating order totals
- Different discounts for different customers
- Easy to switch between strategies

**Example**:
```
// Retail customer
context.setStrategy(new RetailPricing());
total = context.calculateTotal(price, quantity);

// Wholesale customer
context.setStrategy(new WholesalePricing());
total = context.calculateTotal(price, quantity);
```

---

## 📁 Project Structure

```
inventory-system/
│
├── src/
│   └── com/inventory/
│       │
│       ├── model/                    # Entities (Data objects)
│       │   ├── Product.java
│       │   ├── Stock.java
│       │   ├── Category.java
│       │   ├── Supplier.java
│       │   ├── Warehouse.java
│       │   ├── StockMovement.java
│       │   └── User.java
│       │
│       ├── view/                     # GUI (Swing)
│       │   ├── MainFrame.java
│       │   ├── ProductView.java
│       │   ├── StockView.java
│       │   └── DashboardView.java
│       │
│       ├── controller/               # Business Logic
│       │   ├── ProductController.java
│       │   ├── StockController.java
│       │   └── UserController.java
│       │
│       ├── dao/                      # Database Access (DAO Pattern)
│       │   ├── ProductDAO.java       (interface)
│       │   ├── ProductDAOImpl.java   (implementation)
│       │   ├── StockDAO.java
│       │   ├── StockDAOImpl.java
│       │   └── ... (other DAOs)
│       │
│       ├── observer/                 # Observer Pattern
│       │   ├── StockObserver.java    (interface)
│       │   ├── EmailNotifier.java
│       │   ├── DashboardNotifier.java
│       │   └── LogNotifier.java
│       │
│       ├── factory/                  # Factory Pattern
│       │   ├── ProductFactory.java
│       │   ├── SimpleProduct.java
│       │   ├── PerishableProduct.java
│       │   └── SerializedProduct.java
│       │
│       ├── strategy/                 # Strategy Pattern
│       │   ├── PricingStrategy.java   (interface)
│       │   ├── RetailPricing.java
│       │   ├── WholesalePricing.java
│       │   └── VIPPricing.java
│       │
│       ├── singleton/                # Singleton Pattern
│       │   └── DatabaseConnection.java
│       │
│       └── Main.java                 # Entry point
│
└── lib/
    └── mysql-connector-java.jar      # MySQL JDBC driver
```

---

## 🗄️ Entity Relationships

### Simple Entity Diagram

```
Category (1) ──── (N) Product (N) ──── (1) Supplier
                       │
                       │
                  (1) Product (1)
                       │
                       ▼
                  (N) Stock (N) ──── Warehouse (1)
                       │
                       │ (triggers alerts)
                       ▼
                 Observer Pattern
                       │
                       ▼
                  (N) StockMovement (N) ──── User (1)
```

### Entity Descriptions

| Entity | What It Is | Key Fields |
|--------|------------|------------|
| **Product** | Items you sell (can be Simple, Perishable, or Serialized) | id, name, sku, price, categoryId, supplierId |
| **Category** | Product groups | id, name, parentId |
| **Supplier** | Who supplies products | id, name, email, phone |
| **Stock** | Product quantities | id, productId, warehouseId, quantity, minThreshold |
| **Warehouse** | Storage locations | id, name, location |
| **StockMovement** | Transaction log | id, productId, quantity, type, date, userId |
| **User** | System users | id, username, password, role |

### Relationships

- 1 Category → Many Products
- 1 Supplier → Many Products
- 1 Product → Many Stock entries (one per warehouse)
- 1 Warehouse → Many Stock entries
- 1 Stock → Many StockMovements
- 1 User → Many StockMovements

---

## 📋 Component Responsibilities

### MODEL (Entities)

**What they do:**
- Store data (attributes like id, name, price)
- Validate their own data
- Have getters and setters

**What they DON'T do:**
- Save to database
- Know about other entities
- Contain business logic

---

### VIEW (GUI)

**What they do:**
- Display data to user
- Capture user input (forms, buttons)
- Show success/error messages

**What they DON'T do:**
- Access database directly
- Contain business logic
- Process data

---

### CONTROLLER

**What they do:**
- Handle user actions from View
- Call DAO methods to get/save data
- Apply business rules
- Update View with results

**What they DON'T do:**
- Display UI
- Write SQL queries
- Store data

---

### DAO (Data Access)

**What they do:**
- Execute SQL queries
- Convert database rows to objects
- Handle database connections

**What they DON'T do:**
- Validate business rules
- Update UI
- Make business decisions

---

### DatabaseConnection (Singleton)

**What it does:**
- Create ONE database connection
- Share that connection with all DAOs

**What it doesn't do:**
- Execute queries (DAOs do that)
- Store data

---

### Observer Pattern Components

**Stock (Subject):**
- Maintains list of observers
- Notifies them when quantity changes

**Observers (EmailNotifier, etc):**
- Wait for notifications
- Take action when alerted (send email, show alert)

---

### Factory Pattern Component

**ProductFactory:**
- Creates the right product type based on input
- Knows how to construct each type
- Returns ready-to-use product objects

---

### Strategy Pattern Components

**PricingStrategy (interface):**
- Defines how to calculate price

**Concrete Strategies (RetailPricing, etc):**
- Each implements different pricing logic
- Can be swapped at runtime

**PricingContext:**
- Holds current strategy
- Delegates calculation to strategy

---

## 🔄 How Everything Works Together

### Example: User Adds Stock

```
1. User enters quantity in StockView
   ↓
2. StockView calls StockController.addStock()
   ↓
3. StockController:
   - Gets current stock from StockDAO
   - Calculates new quantity
   - Updates stock object
   ↓
4. Stock.setQuantity() is called
   ↓
5. Stock checks: Is new quantity <= minimum?
   ↓
6. If YES → Stock.notifyObservers("LOW_STOCK")
   ↓
7. All observers respond:
   - EmailNotifier → sends email
   - DashboardNotifier → shows alert
   - LogNotifier → writes to log
   ↓
8. StockDAO.update() saves to database
   ↓
9. StockController creates StockMovement record
   ↓
10. StockView.refresh() updates the display
```

---

### Example: Creating a Product (Factory Pattern)

```
1. User fills "Add Product" form in ProductView
   - Selects type: "Perishable"
   - Enters name: "Fresh Milk"
   - Enters expiry date
   ↓
2. ProductView calls ProductController.addProduct(type, data)
   ↓
3. ProductController calls ProductFactory.createProduct("PERISHABLE", data)
   ↓
4. Factory creates PerishableProduct (with expiry date field)
   ↓
5. ProductDAO.save(product)
   ↓
6. Product saved to database
```

**Why this is useful:**
- Don't need if/else everywhere for product types
- Easy to add new types (e.g., "BulkProduct") later
- Creation logic in ONE place

---

### Example: Calculating Price (Strategy Pattern)

```
1. Customer selects 60 items to buy
   ↓
2. System checks customer type: "Wholesale"
   ↓
3. Create pricing context with WholesalePricing strategy
   ↓
4. Calculate: $100 × 60 = $6000
   ↓
5. WholesalePricing applies 10% discount (60 items > 50)
   ↓
6. Final price: $5400
```

**Why this is useful:**
- Switch pricing rules without changing controller code
- Easy to add seasonal sales or promotions
- Customer type determines pricing automatically

---

## 🎯 Design Pattern Summary

| Pattern | Location | Purpose |
|---------|----------|---------|
| **Singleton** | DatabaseConnection | One database connection for entire app |
| **Observer** | Stock monitoring | Auto-alert when stock is low |
| **DAO/Repository** | Data access layer | Separate SQL from business logic |
| **Factory** | Product creation | Create different product types easily |
| **Strategy** | Pricing calculation | Different pricing rules for customers |
| **MVC** | Overall structure | Separate UI, logic, and data |

---

## 📊 Key Principles

### Separation of Concerns
- **View** = What user sees
- **Controller** = What happens when user acts
- **Model** = What data looks like
- **DAO** = How data is saved/loaded

### Single Responsibility
- Each class does ONE thing
- Product class → just holds product data
- ProductDAO → just handles product database operations
- ProductController → just handles product business logic
- ProductView → just displays products

### Don't Repeat Yourself (DRY)
- Database connection code in ONE place (Singleton)
- Alert logic in observers (not scattered everywhere)
- SQL queries in DAOs (not in controllers)

---

## 💡 When to Actually Use These Patterns

### Factory Pattern - Use When:
- You have products with different behaviors (perishable vs. regular)
- You might add more product types later
- You want to avoid messy if/else chains for object creation

**Real scenario**: Food items need expiry tracking, electronics need serial numbers, regular items need nothing special.

---

### Strategy Pattern - Use When:
- You have different ways to calculate something (pricing, discounts)
- Rules change based on context (customer type, season)
- You want to add new pricing rules without changing existing code

**Real scenario**: Retail customers pay full price, wholesale gets bulk discount, VIP members get flat 15% off.

---

## 🔍 Quick Reference

### When to Use Each Component

**Use MODEL when:**
- Representing data (Product, Stock, etc.)

**Use VIEW when:**
- Creating UI forms, tables, buttons

**Use CONTROLLER when:**
- Handling button clicks
- Processing user actions
- Coordinating between View and DAO

**Use DAO when:**
- Saving to database
- Loading from database
- Executing SQL

**Use SINGLETON when:**
- Need exactly one instance (DatabaseConnection)

**Use OBSERVER when:**
- Need automatic notifications (Stock alerts)

**Use FACTORY when:**
- Creating objects with different types (Product types)

**Use STRATEGY when:**
- Need to switch between different algorithms (Pricing rules)

---

## 📝 Summary

This system uses:
1. **MVC** to organize code into View, Controller, Model
2. **Singleton** for single database connection
3. **Observer** for automatic stock alerts
4. **DAO** to separate database code
5. **Factory** to create different product types
6. **Strategy** for flexible pricing rules

**Simple flow:**  
User → View → Controller → DAO → Database → DAO → Controller → View

**That's it!** Keep it simple, follow the patterns, separate your concerns.

---

*Last updated: December 6, 2024*