import pandas as pd
import csv

movie_df = pd.read_csv('movies.csv')
rating = pd.read_csv('ratings.csv')
users = list(set(list(rating['userId'])))
movies = list(set(list(movie_df['movieId'])))
result = []

def getEvaluation(user_id):
  result = { movie: 0.0 for movie in movies }
  user_evalated_movies = set(rating.query("userId == {0}".format(user_id))['movieId'].values)
  
  for movie in user_evalated_movies:
    result[movie] = rating.query("userId == {0} & movieId == {1}".format(user_id, movie)).values[0][2]

  return list(result.values())

for u in users:
  print("operating user {0}".format(u))
  result.append(getEvaluation(u))

with open('matrix.csv', 'w') as file:
  writer = csv.writer(file, lineterminator='\n')
  writer.writerows(result)
