#!/bin/bash
# Helper script to interact with tbc-db via sqlite3
DB="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/project.db"

case "$1" in
  issue-list)
    if [ "$2" = "--assignee" ]; then
      sqlite3 "$DB" "SELECT id, title, status, assignee FROM issues WHERE assignee='$3' ORDER BY created_at DESC;"
    elif [ "$2" = "--status" ]; then
      sqlite3 "$DB" "SELECT id, title, status, assignee FROM issues WHERE status='$3' ORDER BY created_at DESC;"
    else
      sqlite3 "$DB" "SELECT id, title, status, assignee FROM issues WHERE status='open' ORDER BY created_at DESC;"
    fi
    ;;
  issue-view)
    sqlite3 "$DB" "SELECT id, title, body, status, creator, assignee, created_at FROM issues WHERE id=$2;"
    echo ""
    echo "--- Comments ---"
    sqlite3 "$DB" "SELECT author, body, created_at FROM comments WHERE issue_id=$2 ORDER BY created_at;"
    ;;
  issue-create)
    # Extract arguments
    TITLE=""
    BODY=""
    CREATOR=""
    ASSIGNEE=""
    shift # skip command name
    while [[ $# -gt 0 ]]; do
      case $1 in
        --title) TITLE="$2"; shift 2 ;;
        --creator) CREATOR="$2"; shift 2 ;;
        --assignee) ASSIGNEE="$2"; shift 2 ;;
        --body) BODY="$2"; shift 2 ;;
        *) shift ;;
      esac
    done
    # Escape single quotes in SQL
    TITLE="${TITLE//\'/\'\'}"
    BODY="${BODY//\'/\'\'}"
    CREATOR="${CREATOR//\'/\'\'}"
    ASSIGNEE="${ASSIGNEE//\'/\'\'}"
    sqlite3 "$DB" "INSERT INTO issues (title, body, status, creator, assignee, created_at) VALUES ('$TITLE', '$BODY', 'open', '$CREATOR', '$ASSIGNEE', datetime('now')); SELECT last_insert_rowid();"
    ;;
  issue-close)
    sqlite3 "$DB" "UPDATE issues SET status='closed' WHERE id=$2;"
    ;;
  comment)
    # Extract arguments
    ISSUE=""
    AUTHOR=""
    BODY=""
    shift
    while [[ $# -gt 0 ]]; do
      case $1 in
        --issue) ISSUE="$2"; shift 2 ;;
        --author) AUTHOR="$2"; shift 2 ;;
        --body) BODY="$2"; shift 2 ;;
        *) shift ;;
      esac
    done
    AUTHOR="${AUTHOR//\'/\'\'}"
    BODY="${BODY//\'/\'\'}"
    sqlite3 "$DB" "INSERT INTO comments (issue_id, author, body, created_at) VALUES ($ISSUE, '$AUTHOR', '$BODY', datetime('now'));"
    ;;
  *)
    echo "Usage: tbc-db.sh {issue-list|issue-view|issue-create|issue-close|comment}"
    ;;
esac
