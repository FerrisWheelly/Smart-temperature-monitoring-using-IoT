import pandas as pd
import requests
from sklearn.linear_model import LinearRegression
from sklearn.tree import DecisionTreeClassifier

# -------------------------------
# STEP 1: FETCH DATA FROM THINGSPEAK
# -------------------------------
CHANNEL_ID = "****"
READ_API_KEY = "*****"

url = f"https://api.thingspeak.com/channels/{CHANNEL_ID}/feeds.csv?api_key={READ_API_KEY}&results=100"

df = pd.read_csv(url)

# Rename fields
df = df.rename(columns={
"field1": "temperature",
"field2": "humidity"
})

# Clean
df['created_at'] = pd.to_datetime(df['created_at'])
df = df[['created_at', 'temperature', 'humidity']]
df = df.dropna().sort_values('created_at')

# -------------------------------
# STEP 2: FEATURE ENGINEERING
# -------------------------------
df['temp_t-1'] = df['temperature'].shift(1)
df['temp_t-2'] = df['temperature'].shift(2)
df['hum_t-1'] = df['humidity'].shift(1)
df['hum_t-2'] = df['humidity'].shift(2)

df = df.dropna()

# -------------------------------
# STEP 3: TRAIN PREDICTION MODEL
# -------------------------------
X = df[['temp_t-1','temp_t-2','hum_t-1','hum_t-2']]
y = df[['temperature','humidity']]

model = LinearRegression()
model.fit(X, y)

# -------------------------------
# STEP 4: CREATE LABELS
# -------------------------------
def label_func(row):
    if row['temperature'] > 30:
        return 1
    elif row['temperature'] > 28 and row['humidity'] < 40:
     return 1
    else:
      return 0

df['label'] = df.apply(label_func, axis=1)

# -------------------------------
# STEP 5: TRAIN DECISION MODEL
# -------------------------------
clf = DecisionTreeClassifier()
clf.fit(df[['temperature','humidity']], df['label'])

# -------------------------------
# STEP 6: GET LATEST VALUES FROM THINGSPEAK
# -------------------------------
last = df.iloc[-1]
prev = df.iloc[-2]

input_df = pd.DataFrame([[
last['temperature'],
prev['temperature'],
last['humidity'],
prev['humidity']
]], columns=['temp_t-1','temp_t-2','hum_t-1','hum_t-2'])

# -------------------------------
# STEP 7: PREDICT FUTURE
# -------------------------------
future = model.predict(input_df)

future_temp = future[0][0]
future_humidity = future[0][1]

print("\n Predicted Next Values:")
print(f"Temperature: {future_temp:.2f}")
print(f" Humidity: {future_humidity:.2f}")

# -------------------------------
# STEP 8: SMART DECISION
# -------------------------------
input_df2 = pd.DataFrame([[future_temp, future_humidity]],
columns=['temperature','humidity'])

decision = clf.predict(input_df2)

# -------------------------------
# STEP 9: FINAL OUTPUT
# -------------------------------
if decision == 1:
    print(" Motor ON (High Temp Predicted)")
    motor_state = 1
else:
    print("Motor OFF (Normal)")
    motor_state = 0
