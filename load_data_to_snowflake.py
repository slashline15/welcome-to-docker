import snowflake.connector


# Snowflake connector
conn = snowflake.connector.connect(
    user='YOUR_USER',
    password='YOUR_PASSWORD',
    account='YOUR_ACCOUNT',
    warehouse='YOUR_WAREHOUSE',
    database='YOUR_DATABASE',
    schema='YOUR_SCHEMA'
)


# Load Data Operations
def load_data_to_snowflake(data):
    cursor = conn.cursor()
    cursor.execute("""
        INSERT INTO your_table (column1, column2) VALUES (%s, %s)
    """, (data['column1'], data['column2']))
    cursor.close()


# Call this function to load Data from Kafka message to the Snowflake