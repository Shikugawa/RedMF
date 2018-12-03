SELECT *
FROM scores 
INNER JOIN projects ON projects.id = scores.project_id
WHERE projects.id = 1
ORDER BY scores.score DESC

Score.joins(:projects)
     .where("projects.id = ?", 1)
     .order("scores.score DESC")