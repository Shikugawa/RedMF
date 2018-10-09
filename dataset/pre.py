import pandas as pd
import csv

movie_df = pd.read_csv('movies.csv')
rating = pd.read_csv('ratings.csv')
users = list(set(list(rating['userId'])))
movies = list(set(list(movie_df['movieId'])))
result = []

def getEvaluation(user_id, movie_id):
  data = rating.query("userId == {0} & movieId == {1}".format(user_id, movie_id)).values
  if len(data) != 0:
    return data[0][2]
  else:
    return 0.0

for u in users:
  print("operating user {0}".format(u))
  ratings_per_user = []
  for i in movies:
    ratings_per_user.append(getEvaluation(u, i))
  result.append(ratings_per_user)

with open('matrix.csv', 'w') as file:
  writer = csv.writer(file, lineterminator='\n')
  writer.writerows(result)
  