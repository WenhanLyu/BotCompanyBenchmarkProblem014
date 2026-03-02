#!/bin/bash
# Helper wrapper for tbc-db operations using sqlite3
DB="${TBC_DB:-/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/project.db}"

case "$1" in
    issue-list)
        sqlite3 "$DB" "SELECT id || ' | ' || title || ' | ' || status || ' | ' || assignee FROM issues ORDER BY created_at DESC;"
        ;;
    issue-view)
        sqlite3 "$DB" "SELECT 'ID: ' || id || '\nTitle: ' || title || '\nStatus: ' || status || '\nCreator: ' || creator || '\nAssignee: ' || assignee || '\n\n' || body FROM issues WHERE id=$2;"
        sqlite3 "$DB" "SELECT '\n--- Comments ---\n' || group_concat(author || ' (' || created_at || '):\n' || body, '\n\n') FROM comments WHERE issue_id=$2;"
        ;;
    comments)
        sqlite3 "$DB" "SELECT author || ' (' || created_at || '):\n' || body FROM comments WHERE issue_id=$2 ORDER BY created_at;"
        ;;
    *)
        echo "Usage: tbc-db.sh {issue-list|issue-view|comments} [args]"
        ;;
esac
